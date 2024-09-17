//主要实现语音识别子服务的服务器的搭建
#include "gateway_server.hpp"

DEFINE_bool(run_mode, false, "程序的运行模式，false-调试； true-发布；");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志输出等级");

DEFINE_int32(http_listen_port, 9000, "HTTP服务器监听端口");
DEFINE_int32(websocket_listen_port, 9001, "Websocket服务器监听端口");

DEFINE_string(registry_host, "http://127.0.0.1:2379", "服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(file_service, "/service/file_service", "文件存储子服务名称");
DEFINE_string(friend_service, "/service/friend_service", "好友管理子服务名称");
DEFINE_string(message_service, "/service/message_service", "消息存储子服务名称");
DEFINE_string(user_service, "/service/user_service", "用户管理子服务名称");
DEFINE_string(speech_service, "/service/speech_service", "语音识别子服务名称");
DEFINE_string(transmit_service, "/service/transmit_service", "转发管理子服务名称");

DEFINE_string(redis_host, "127.0.0.1", "Redis服务器访问地址");
DEFINE_int32(redis_port, 6379, "Redis服务器访问端口");
DEFINE_int32(redis_db, 0, "Redis默认库号");
DEFINE_bool(redis_keep_alive, true, "Redis长连接保活选项");
DEFINE_string(redis_password, "zpyredis123", "Redis密码");
int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    chat_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    chat_im::GatewayServerBuilder gsb;
    gsb.make_redis_object(FLAGS_redis_host, FLAGS_redis_port, FLAGS_redis_db, FLAGS_redis_keep_alive,FLAGS_redis_password);
    gsb.make_discovery_object(FLAGS_registry_host, FLAGS_base_service, FLAGS_file_service,
        FLAGS_speech_service, FLAGS_message_service, FLAGS_friend_service, 
        FLAGS_user_service, FLAGS_transmit_service);
    gsb.make_server_object(FLAGS_websocket_listen_port, FLAGS_http_listen_port);
    auto server = gsb.build();
    server->start();
    return 0;
}