#include <brpc/server.h>
#include <butil/logging.h>
#include "main.pb.h"
#include "main.pb.cc"
// 1.创建子类，继承EchoService，实现rpc调用的业务功能
class EchoServiceImpl : public example::EchoService {
public:
    virtual void Echo(google::protobuf::RpcController* controller,
                       const ::example::EchoRequest* request,
                       ::example::EchoResponse* response,
                       ::google::protobuf::Closure* done) {
        brpc::ClosureGuard rpc_guard(done);
        std::cout << request->message() << std::endl;
        std::string str = request->message() + "--这是响应！";
        response->set_message(str);
    }
private:
};
int main(int argc, char* argv[]) {
    brpc::Server server;
    EchoServiceImpl echo_service;
    server.AddService(&echo_service, brpc::ServiceOwnership::SERVER_DOESNT_OWN_SERVICE);
    brpc::ServerOptions options;
    options.idle_timeout_sec = -1;   //连接空闲超时事件，超时后释放
    options.num_threads = 1;
    server.Start(8081, &options);
    server.RunUntilAskedToQuit();   //睡眠直到ctrl + c
    return 0;
}

// 2.构造服务器对象

// 3.向服务器对象中，新增EchoService服务

// 4.启动服务器