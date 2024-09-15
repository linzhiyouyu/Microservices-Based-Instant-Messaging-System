#pragma once
#include <brpc/channel.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex> 
#include "logger.hpp"
namespace chat_im {
    class ServiceChannel {
    public:
        using ptr = std::shared_ptr<ServiceChannel>;
        using ChannelPtr = std::shared_ptr<brpc::Channel>;
        ServiceChannel(const std::string& name)
            :_service_name(name), _idx(0) {}
        ~ServiceChannel(){}
    public:
        void append(const std::string& host) {
            auto node = std::make_shared<brpc::Channel>();
            brpc::ChannelOptions ops;
            ops.connect_timeout_ms = -1;
            ops.timeout_ms = -1;
            ops.max_retry = 3;
            ops.protocol = "baidu_std";
            if(node->Init(host.c_str(), &ops) == -1) {
                ERROR("初始化{}-{}信道失败", _service_name, host);
                return;
            }
            std::unique_lock lock(_mtx);
            _hosts[host] = node;    
            _nodes.push_back(node);
        }
        void remove(const std::string& host) {    //直接原地删除，挪动数据，因为节点不可能很多，不影响效率
            std::unique_lock lock(_mtx);
            auto it = _hosts.find(host);
            if(it == _hosts.end()) {
                WARN("{}-{}节点删除信道时无法找到对应信道信息", _service_name, host);
                return;
            }
            for(auto x = _nodes.begin(); x != _nodes.end(); ++x) {
                if(*x != it->second)
                    continue;
                _nodes.erase(x);
                break;
            }
            _hosts.erase(it);
        }
        ChannelPtr choose() {   //采用RR轮转策略，选择最近最少提供服务的节点来提供服务，尽量保证负载均衡
            std::unique_lock lock(_mtx);
            if(_nodes.size() == 0) {
                ERROR("当前没有能够提供{}服务的节点", _service_name);
                return nullptr;
            }
            int idx = _idx;
            _idx = (_idx + 1) % _nodes.size();
            return _nodes[idx];
        }
    private:
        int _idx;   //RR轮转指针
        std::string _service_name;   //服务名称
        std::vector<ChannelPtr> _nodes;   //服务节点
        std::unordered_map<std::string, ChannelPtr> _hosts;   //节点->信道
        std::mutex _mtx;
    };

    //总体信道服务管理类
    class ServiceManager {
    public:
        using ptr = std::shared_ptr<ServiceManager>;
        //获取服务的节点信道
        ServiceChannel::ChannelPtr choose(const std::string& service_name) {
            std::unique_lock lock(_mtx);
            auto it = _services.find(service_name);
            if(it == _services.end()) {
                ERROR("当前没有能够提供{}服务的节点", service_name);
                return nullptr;
            }
            return it->second->choose();
        }
        //声明关注哪些服务的上下线，不关心的就不管理
        void declared(const std::string& service_name) {
            std::unique_lock lock(_mtx);
            _follow_service.insert(service_name);
        }
        void onServiceOnline(const std::string& service_instance, const std::string& host) {
            std::string service_name = getServiceName(service_instance);
            ServiceChannel::ptr service;
            {
                std::unique_lock lock(_mtx);
                auto it = _follow_service.find(service_name);
                if(it == _follow_service.end()) {
                    DEBUG("{}-{}服务上线了，但是当前并不关心", service_name, host);
                    return;
                }
                auto sit = _services.find(service_name);
                if(sit == _services.end()) {
                    service = std::make_shared<ServiceChannel>(service_name);
                    _services.emplace(service_name, service);
                } else 
                    service = sit->second;
            } 
            if(!service) {
                ERROR("新增{}服务管理节点失败!", service_name);
                return;
            }
            service->append(host);
        }
        void onServiceOffline(const std::string& service_instance, const std::string& host) {
            std::string service_name = getServiceName(service_instance);
            ServiceChannel::ptr service;
            {
                std::unique_lock lock(_mtx);
                auto it = _follow_service.find(service_name);
                if(it == _follow_service.end()) {
                    DEBUG("{}-{}服务下线了，但是当前并不关心", service_name, host);
                    return;
                }
                auto sit = _services.find(service_name);
                if(sit == _services.end()) {
                    WARN("删除{}服务节点时，没有找到管理对象", service_name);
                    return;
                } 
                service = sit->second;
            }
            service->remove(host);
        }
    private:
        std::string getServiceName(const std::string& service_instance) {
            auto pos = service_instance.find_last_of('/');
            return service_instance.substr(0, pos);
        }
    private:
        std::mutex _mtx;
        std::unordered_set<std::string> _follow_service;
        std::unordered_map<std::string, ServiceChannel::ptr> _services;
    };
}