#include <brpc/channel.h>
#include "main.pb.h"
#include "main.pb.cc"
// void callback() {

// }

int main(int argc, char* argv[]) {
    //1. 构造channel信道
    brpc::ChannelOptions options;
    options.connect_timeout_ms = -1;
    options.timeout_ms = -1;
    options.max_retry = 3;
    options.protocol = "baidu_std";
    brpc::Channel channel;
    channel.Init("127.0.0.1:8081", &options);
    //2. 构造EchoService_Stub，用于进行rpc调用
    example::EchoService_Stub stub(&channel);
    example::EchoRequest req;
    req.set_message("你好");
    brpc::Controller cntl;
    example::EchoResponse rsp;
    stub.Echo(&cntl, &req, &rsp, nullptr);
    if(cntl.Failed() == true) {
        std::cout << "rpc调用失败" << cntl.ErrorText() << std::endl;
        return -1;
    }
    std::cout << "收到响应" << rsp.message() << std::endl; 
    return 0;
}