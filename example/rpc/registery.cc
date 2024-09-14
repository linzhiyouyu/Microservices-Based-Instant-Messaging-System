#include "../common/etcd.hpp"
#include <gflags/gflags.h>
#include "main.pb.h"
#include "main.pb.cc"
#include <brpc/server.h>
#include <butil/logging.h>
#include <thread>
DEFINE_bool(run_mode, false, "日志的运行模式, false - 调试模式, true - 发布模式");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志的输出等级");
DEFINE_string(etcd_host, "http://127.0.0.1:2379", "服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(instance_name, "/echo/instance", "当前客户端实例名称");
DEFINE_string(access_host, "127.0.0.1:9090", "当前实例的外部访问地址");
DEFINE_int32(listen_port, 9090, "RPC服务器监听端口");

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
    google::ParseCommandLineFlags(&argc, &argv, true);
    init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);
    
    logging::LoggingSettings settings;
    settings.logging_dest = logging::LoggingDestination::LOG_TO_NONE;
    logging::InitLogging(settings); 
    brpc::Server server;
    EchoServiceImpl echo_service;
    server.AddService(&echo_service, brpc::ServiceOwnership::SERVER_DOESNT_OWN_SERVICE);
    brpc::ServerOptions options;
    options.idle_timeout_sec = -1;   //连接空闲超时事件，超时后释放
    options.num_threads = 1;
    server.Start(FLAGS_listen_port, &options);
    Registry::ptr rclient = std::make_shared<Registry>(FLAGS_etcd_host);
    rclient->registry(FLAGS_base_service + FLAGS_instance_name, FLAGS_access_host);
    std::this_thread::sleep_for(std::chrono::seconds(600));
    server.RunUntilAskedToQuit();   //睡眠直到ctrl + c
    return 0;
}