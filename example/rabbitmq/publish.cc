#include "../../common/rabbitmq.hpp"
#include <gflags/gflags.h>
DEFINE_string(user, "root", "这是服务器的监听地址，格式: 127.0.0.1");
DEFINE_string(host, "127.0.0.1:5672", "这是服务器的监听地址，格式: 127.0.0.1");
DEFINE_string(pswd, "123456", "这是服务器的监听端口，格式: 8080");
DEFINE_bool(run_mode, false, "日志的运行模式, false - 调试模式, true - 发布模式");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志的输出等级");
int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);
    MQClient client(FLAGS_user, FLAGS_pswd, FLAGS_host);
    client.declareComponents("test_exchange", "test_queue");
    for(int i = 0; i < 10; ++i) {
        std::string msg = "Hello: " + std::to_string(i);
        if(client.publish("test_exchange", msg) == false) 
            std::cout << "消息发布失败" << std::endl;
    } 
    std::this_thread::sleep_for(std::chrono::seconds(60));
    
    return 0;
}