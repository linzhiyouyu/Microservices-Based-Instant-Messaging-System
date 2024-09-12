#pragma once
#include "logger.hpp"
#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Response.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/Value.hpp>
#include <memory>
#include <string>
#include <functional>

using namespace lcz_zpy;
//服务注册客户端类
class Registry {
public:
    using ptr = std::shared_ptr<Registry>;
    Registry(const std::string& host)
        :_client(std::make_shared<etcd::Client>(host))
        ,_keep_alive(_client->leasekeepalive(3).get())
        ,_lease_id(_keep_alive->Lease())
    {}
    ~Registry() { _keep_alive->Cancel(); }
    bool registry(const std::string& key, const std::string& val) {
        auto resp = _client->put(key, val, _lease_id).get();
        if(resp.is_ok() == false) {
            DEBUG("注册数据失败: {}", resp.error_message());
            return false;
        }
        return true;
    }
private:
    std::shared_ptr<etcd::Client> _client;
    std::shared_ptr<etcd::KeepAlive> _keep_alive;
    uint64_t  _lease_id;    //租约id
};

//服务发现客户端类
class Discovery {
public:
    using ptr = std::shared_ptr<Discovery>;
    using notify = std::function<void(std::string, std::string)>;
    Discovery(const std::string& host, const std::string& basedir, const notify& put_cb, const notify& del_cb)
        :_client(std::make_shared<etcd::Client>(host))
        ,_put_cb(put_cb)
        ,_del_cb(del_cb) {
        
        //1. 进行服务发现
        auto resp = _client->ls(basedir).get();
        if(resp.is_ok() == false) {
            DEBUG("获取服务信息数据失败: {}", resp.error_message());
        }
        int sz = resp.keys().size();
        for(int i = 0; i < sz; ++i)
            if(_put_cb)
                _put_cb(resp.key(i), resp.value(i).as_string());
        //2. 然后进行事件监控，监控数据发生的改变并调用对应回调来处理 
        //_watcher = std::make_shared<etcd::Watcher>(*_client.get(), basedir, std::bind(&Discovery::callback, std::placeholders::_1), true);
    }
private:
    // void callback(const etcd::Response& resp) {
    //     if(resp.is_ok() == false) {
    //         ERROR("收到一个错误的事件通知: {}", resp.error_message());
    //         return;
    //     }
    //     for(auto&& ev : resp.events()) {
    //         if(ev.event_type() == etcd::Event::EventType::PUT) {
    //             if(_put_cb)
    //                 _put_cb(ev.kv().key(), ev.kv().as_string());
    //             DEBUG("新增服务: {}-{}", ev.kv().key(), ev.kv().as_string());
    //         } else if(ev.event_type() == etcd::Event::EventType::DELETE_) { 
    //             if(_del_cb)
    //                 _del_cb(ev.prev_kv().key(), ev.prev_kv().as_string());
    //             DEBUG("下线服务: {}-{}", ev.prev_kv().key(), ev.prev_kv().as_string());
    //         }
    //     }
    // }
private:
    notify _put_cb;
    notify _del_cb;
    std::shared_ptr<etcd::Client> _client;
    std::shared_ptr<etcd::Watcher> _watcher;
};