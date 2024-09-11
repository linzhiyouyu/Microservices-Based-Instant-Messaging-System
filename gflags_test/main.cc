#include <gflags/gflags.h>
#include <iostream>
DEFINE_string(ip, "127.0.0.1", "这是服务器的监听地址，格式: 127.0.0.1");
DEFINE_int32(port, 8080, "这是服务器的监听端口，格式: 8080");
DEFINE_bool(debug_enable, true, "是否启用调试模式，格式: true/false");
int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << FLAGS_ip << std::endl;
    std::cout << FLAGS_port << std::endl;
    std::cout << FLAGS_debug_enable << std::endl;
    return 0; 
}