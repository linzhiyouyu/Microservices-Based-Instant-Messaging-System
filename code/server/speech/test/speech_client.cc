//speech_server的测试客户端实现
//1. 进行服务发现--发现speech_server的服务器节点地址信息并实例化的通信信道
//2. 读取语音文件数据
//3. 发起语音识别RPC调用

#include "etcd.hpp"
#include "channel.hpp"
#include <gflags/gflags.h>
#include <thread>
#include "aip-cpp-sdk/speech.h"
#include "speech.pb.h"


DEFINE_bool(run_mode, false, "程序的运行模式，false-调试； true-发布；");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志输出等级");

DEFINE_string(etcd_host, "http://127.0.0.1:2379", "服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(speech_service, "/service/speech_service", "服务监控根目录");

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    chat_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    
    //1. 先构造Rpc信道管理对象
    auto sm = std::make_shared<chat_im::ServiceManager>();
    sm->declared(FLAGS_speech_service);
    auto put_cb = std::bind(&chat_im::ServiceManager::onServiceOnline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    auto del_cb = std::bind(&chat_im::ServiceManager::onServiceOffline, sm.get(), std::placeholders::_1, std::placeholders::_2);
    //2. 构造服务发现对象
    chat_im::Discovery::ptr dclient = std::make_shared<chat_im::Discovery>(FLAGS_etcd_host, FLAGS_base_service, put_cb, del_cb);
    
    //3. 通过Rpc信道管理对象，获取提供Echo服务的信道
    auto channel = sm->choose(FLAGS_speech_service);
    if (!channel) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }
    //读取语音文件数据
    std::string file_content;
    aip::get_file_content("16k.pcm", &file_content);
    std::cout << file_content.size() << std::endl;

    //4. 发起EchoRpc调用
    chat_im::SpeechService_Stub stub(channel.get());
    chat_im::SpeechRecognitionReq req;
    req.set_speech_content(file_content);
    req.set_request_id("111111");

    brpc::Controller *cntl = new brpc::Controller();
    chat_im::SpeechRecognitionRsp *rsp = new chat_im::SpeechRecognitionRsp();
    stub.SpeechRecognition(cntl, &req, rsp, nullptr);
    if (cntl->Failed() == true) {
        std::cout << "Rpc调用失败：" << cntl->ErrorText() << std::endl;
        delete cntl;
        delete rsp;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }
    if (rsp->success() == false) {
        std::cout << rsp->errmsg() << std::endl;
        return -1;
    }
    std::cout << "收到响应: " << rsp->request_id() << std::endl;
    std::cout << "收到响应: " << rsp->recognition_result() << std::endl;
    return 0;
}