#include "file_server.hpp"


DEFINE_bool(run_mode, false, "日志的运行模式, false - 调试模式, true - 发布模式");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志的输出等级");
DEFINE_string(registry_host, "http://127.0.0.1:2379", "服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(instance_name, "/file_service/instance", "当前客户端实例名称");
DEFINE_string(access_host, "0.0.0.0:10002", "当前实例的外部访问地址");
DEFINE_int32(listen_port, 10002, "RPC服务器监听端口");
DEFINE_int32(rpc_timeout, -1, "RPC调用超时时间");
DEFINE_int32(rpc_threads, 1, "RPC的IO线程数量");



int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    chat_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);
    chat_im::FileServerBuilder fsb;
    fsb.make_rpc_server(FLAGS_listen_port, FLAGS_rpc_timeout, FLAGS_rpc_threads);
    fsb.make_reg_object(FLAGS_registry_host, FLAGS_base_service + FLAGS_instance_name, FLAGS_access_host);
    auto server = fsb.build();
    server->start();
    return 0;
}