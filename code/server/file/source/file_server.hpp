#pragma once
#include <brpc/server.h>
#include <butil/logging.h>
#include "etcd.hpp"
#include "logger.hpp"
#include "base.pb.h"
#include "file.pb.h"
#include "utils.hpp"

namespace chat_im {
    class FileServerImpl : public chat_im::FileService {
    public:
        FileServerImpl() {}
        ~FileServerImpl() {}
        void GetSingleFile(google::protobuf::RpcController* controller, 
                            const ::chat_im::GetSingleFileReq* request, 
                            ::chat_im::GetSingleFileRsp* response, 
                            ::google::protobuf::Closure* done) {
            brpc::ClosureGuard rpc_guard(done);
            response->set_request_id(request->request_id());
            std::string fid = request->file_id();
            std::string body;
            if(!readFile(fid, body)) {
                response->set_success(false);
                response->set_errmsg("读取文件数据失败!");
                ERROR("{}读取文件数据失败!", request->request_id());
                return;
            }
            response->set_success(true);
            response->mutable_file_data()->set_file_id(fid);
            response->mutable_file_data()->set_file_content(body);
        }
        void GetMultiFile(google::protobuf::RpcController* controller, 
                            const ::chat_im::GetMultiFileReq* request, 
                            ::chat_im::GetMultiFileRsp* response, 
                            ::google::protobuf::Closure* done) {
            brpc::ClosureGuard rpc_guard(done);
            //循环取出请求中的ID，读取文件中的数据进行填充
            for(int i = 0; i < request->file_id_list_size(); ++i) {
                std::string fid = request->file_id_list(i);
                std::string body;
                if(!readFile(fid, body)) {
                    response->set_success(false);
                    response->set_errmsg("读取文件数据失败!");
                    ERROR("{}读取文件数据失败!", request->request_id());
                    return;    //有一个失败，本次请求判为全部失败
                }
                FileDownloadData data;
                data.set_file_id(fid);
                data.set_file_content(body);
                response->mutable_file_data()->emplace(fid, data);
            }
            response->set_success(true);
        }
        void PutSingleFile(google::protobuf::RpcController* controller, 
                            const ::chat_im::PutSingleFileReq* request, 
                            ::chat_im::PutSingleFileRsp* response, 
                            ::google::protobuf::Closure* done) {
            brpc::ClosureGuard rpc_guard(done);
            response->set_request_id(request->request_id());
            //1.为文件生成一个唯一uuid作为文件名
            std::string fid = uuid();
            //2. 取出请求中的文件数据，进行文件数据写入
            if(!writeFile(fid, request->file_data().file_content())) {
                response->set_success(false);
                response->set_errmsg("写入文件数据失败!");
                ERROR("{}写入文件数据失败!", request->request_id());
                return;
            }
            //3. 组织响应
            response->set_success(true);
            response->mutable_file_info()->set_file_id(fid);
            response->mutable_file_info()->set_file_size(request->file_data().file_size());
            response->mutable_file_info()->set_file_name(request->file_data().file_name());
        }
        void PutMultiFile(google::protobuf::RpcController* controller, 
                            const ::chat_im::PutMultiFileReq* request, 
                            ::chat_im::PutMultiFileRsp* response, 
                            ::google::protobuf::Closure* done) {
            brpc::ClosureGuard rpc_guard(done);
            response->set_request_id(request->request_id());
            for(int i = 0; i < request->file_data_size(); ++i) {
                std::string fid = uuid();
                if(!writeFile(fid, request->file_data(i).file_content())) {
                    response->set_success(false);
                    response->set_errmsg("写入文件数据失败!");
                    ERROR("{}写入文件数据失败!", request->request_id());
                    return;
                }
                FileMessageInfo *info = response->add_file_info();
                info->set_file_id(fid);
                info->set_file_size(request->file_data(i).file_size());
                info->set_file_name(request->file_data(i).file_name());
            }
            response->set_success(true);
        }
    };
    class FileServer {
    public:
        using ptr = std::shared_ptr<FileServer>;
        FileServer(const Registry::ptr& reg_client,
                    const std::shared_ptr<brpc::Server>& server) {}
        ~FileServer() {}
        
        void start() {
            _rpc_server->RunUntilAskedToQuit();
        }
    private:
        Registry::ptr _reg_client;
        std::shared_ptr<brpc::Server> _rpc_server;
    };
    //建造者模式，将建造权掌握在手里，不让外部随便调用，这样可以在服务端内部顺利完成所有初始化后再往后走
    class FileServerBuilder {
    public:
        void make_reg_object(const std::string& reg_host, 
                            const std::string& service_name,
                            const std::string& access_host) {
            _reg_client = std::make_shared<Registry>(reg_host);
            _reg_client->registry(service_name, access_host);

        }
        void make_rpc_server(uint16_t port, int32_t timeout, uint8_t num_threads) {
            _rpc_server = std::make_shared<brpc::Server>();
            FileServerImpl *file_service = new FileServerImpl();
            int ret = _rpc_server->AddService(file_service, brpc::ServiceOwnership::SERVER_OWNS_SERVICE);
            if(ret == -1) {
                ERROR("添加RPC服务失败!");
                abort();
            }
            brpc::ServerOptions options;
            options.idle_timeout_sec = timeout;
            options.num_threads = num_threads;
            ret = _rpc_server->Start(port, &options);
            if(ret == -1) {
                ERROR("服务器启动失败!");
                abort();
            }
        }
        FileServer::ptr build() {
            if(!_reg_client) {
                ERROR("未初始化服务注册模块");
                abort();
            }
            if(!_rpc_server) {
                ERROR("未初始化RPC服务器模块");
                abort();
            }
            FileServer::ptr server = std::make_shared<FileServer>(_reg_client, _rpc_server);
            return server;
        }
    private:
        Registry::ptr _reg_client;
        std::shared_ptr<brpc::Server> _rpc_server;
    };
}