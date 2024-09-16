#include "etcd.hpp"
#include "channel.hpp"
#include "utils.hpp"
#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <thread>
#include "user.pb.h"
#include "base.pb.h"

DEFINE_bool(run_mode, false, "程序的运行模式，false-调试； true-发布；");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志输出等级");

DEFINE_string(etcd_host, "http://127.0.0.1:2379", "服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(user_service, "/service/user_service", "服务监控根目录");

chat_im::ServiceManager::ptr _user_channels;

chat_im::UserInfo user_info;

std::string login_ssid;
std::string new_nickname = "亲爱的猪妈妈";

// TEST(用户子服务测试, 用户注册测试) {
//     auto channel = _user_channels->choose(FLAGS_user_service);//获取通信信道
//     ASSERT_TRUE(channel);
//     user_info.set_nickname("猪爸爸");

//     chat_im::UserRegisterReq req;
//     req.set_request_id(chat_im::uuid());
//     req.set_nickname(user_info.nickname());
//     req.set_password("123456");
//     chat_im::UserRegisterRsp rsp;
//     brpc::Controller cntl;
//     chat_im::UserService_Stub stub(channel.get());
//     stub.UserRegister(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }

// TEST(用户子服务测试, 用户登录测试) {
//     auto channel = _user_channels->choose(FLAGS_user_service);//获取通信信道
//     ASSERT_TRUE(channel);

//     chat_im::UserLoginReq req;
//     req.set_request_id(chat_im::uuid());
//     req.set_nickname("猪妈妈");
//     req.set_password("123456");
//     chat_im::UserLoginRsp rsp;
//     brpc::Controller cntl;
//     chat_im::UserService_Stub stub(channel.get());
//     stub.UserLogin(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
//     login_ssid = rsp.login_session_id();
// }
// TEST(用户子服务测试, 用户头像设置测试) {
//     auto channel = _user_channels->choose(FLAGS_user_service);//获取通信信道
//     ASSERT_TRUE(channel);

//     chat_im::SetUserAvatarReq req;
//     req.set_request_id(chat_im::uuid());
//     req.set_user_id(user_info.user_id());
//     req.set_session_id(login_ssid);
//     req.set_avatar(user_info.avatar());
//     chat_im::SetUserAvatarRsp rsp;
//     brpc::Controller cntl;
//     chat_im::UserService_Stub stub(channel.get());
//     stub.SetUserAvatar(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }

// TEST(用户子服务测试, 用户签名设置测试) {
//     auto channel = _user_channels->choose(FLAGS_user_service);//获取通信信道
//     ASSERT_TRUE(channel);

//     chat_im::SetUserDescriptionReq req;
//     req.set_request_id(chat_im::uuid());
//     req.set_user_id(user_info.user_id());
//     req.set_session_id(login_ssid);
//     req.set_description(user_info.description());
//     chat_im::SetUserDescriptionRsp rsp;
//     brpc::Controller cntl;
//     chat_im::UserService_Stub stub(channel.get());
//     stub.SetUserDescription(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }

// TEST(用户子服务测试, 用户昵称设置测试) {
//     auto channel = _user_channels->choose(FLAGS_user_service);//获取通信信道
//     ASSERT_TRUE(channel);

//     chat_im::SetUserNicknameReq req;
//     req.set_request_id(chat_im::uuid());
//     req.set_user_id(user_info.user_id());
//     req.set_session_id(login_ssid);
//     req.set_nickname(new_nickname);
//     chat_im::SetUserNicknameRsp rsp;
//     brpc::Controller cntl;
//     chat_im::UserService_Stub stub(channel.get());
//     stub.SetUserNickname(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }


// TEST(用户子服务测试, 用户信息获取测试) {
//     auto channel = _user_channels->choose(FLAGS_user_service);//获取通信信道
//     ASSERT_TRUE(channel);

//     chat_im::GetUserInfoReq req;
//     req.set_request_id(chat_im::uuid());
//     req.set_user_id(user_info.user_id());
//     req.set_session_id(login_ssid);
//     chat_im::GetUserInfoRsp rsp;
//     brpc::Controller cntl;
//     chat_im::UserService_Stub stub(channel.get());
//     stub.GetUserInfo(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
//     ASSERT_EQ(user_info.user_id(), rsp.user_info().user_id());
//     ASSERT_EQ(new_nickname, rsp.user_info().nickname());
//     ASSERT_EQ(user_info.description(), rsp.user_info().description());
//     ASSERT_EQ("", rsp.user_info().phone());
//     ASSERT_EQ(user_info.avatar(), rsp.user_info().avatar());
// }

// void set_user_avatar(const std::string &uid, const std::string &avatar) {
//     auto channel = _user_channels->choose(FLAGS_user_service);//获取通信信道
//     ASSERT_TRUE(channel);
//     chat_im::SetUserAvatarReq req;
//     req.set_request_id(chat_im::uuid());
//     req.set_user_id(uid);
//     req.set_session_id(login_ssid);
//     req.set_avatar(avatar);
//     chat_im::SetUserAvatarRsp rsp;
//     brpc::Controller cntl;
//     chat_im::UserService_Stub stub(channel.get());
//     stub.SetUserAvatar(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }

// TEST(用户子服务测试, 批量用户信息获取测试) {
//     set_user_avatar("用户ID1", "小猪佩奇的头像数据");
//     set_user_avatar("用户ID2", "小猪乔治的头像数据");
//     auto channel = _user_channels->choose(FLAGS_user_service);//获取通信信道
//     ASSERT_TRUE(channel);

//     chat_im::GetMultiUserInfoReq req;
//     req.set_request_id(chat_im::uuid());
//     req.add_users_id("用户ID1");
//     req.add_users_id("用户ID2");
//     req.add_users_id("123");
//     chat_im::GetMultiUserInfoRsp rsp;
//     brpc::Controller cntl;
//     chat_im::UserService_Stub stub(channel.get());
//     stub.GetMultiUserInfo(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
//     ASSERT_EQ(rsp.users_info_size(), 3);
    
//     auto users_map = rsp.mutable_users_info();
//     chat_im::UserInfo fuser = (*users_map)["123"];
//     ASSERT_EQ(fuser.user_id(), "123");
//     ASSERT_EQ(fuser.nickname(), "猪爸爸");
//     ASSERT_EQ(fuser.description(), "");
//     ASSERT_EQ(fuser.phone(), "");
//     ASSERT_EQ(fuser.avatar(), "");

//     chat_im::UserInfo puser = (*users_map)["用户ID1"];
//     ASSERT_EQ(puser.user_id(), "用户ID1");
//     ASSERT_EQ(puser.nickname(), "小猪佩奇");
//     ASSERT_EQ(puser.description(), "这是一只小猪");
//     ASSERT_EQ(puser.phone(), "6666");
//     ASSERT_EQ(puser.avatar(), "小猪佩奇的头像数据");
    
//     chat_im::UserInfo quser = (*users_map)["用户ID2"];
//     ASSERT_EQ(quser.user_id(), "用户ID2");
//     ASSERT_EQ(quser.nickname(), "小猪乔治");
//     ASSERT_EQ(quser.description(), "这是一只小小猪");
//     ASSERT_EQ(quser.phone(), "8888");
//     ASSERT_EQ(quser.avatar(), "小猪乔治的头像数据");

    
// }

std::string code_id;
void get_code() {
    auto channel = _user_channels->choose(FLAGS_user_service);//获取通信信道
    ASSERT_TRUE(channel);

    chat_im::PhoneVerifyCodeReq req;
    req.set_request_id(chat_im::uuid());
    req.set_phone_number(user_info.phone());
    chat_im::PhoneVerifyCodeRsp rsp;
    brpc::Controller cntl;
    chat_im::UserService_Stub stub(channel.get());
    stub.GetPhoneVerifyCode(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    code_id = rsp.verify_code_id();
}


// TEST(用户子服务测试, 手机号注册) {
//     get_code();
//     auto channel = _user_channels->choose(FLAGS_user_service);
//     ASSERT_TRUE(channel);

//     chat_im::PhoneRegisterReq req;
//     req.set_request_id(chat_im::uuid());
//     req.set_phone_number(user_info.phone());
//     req.set_verify_code_id(code_id);
//     std::cout << "手机号注册，输入验证码：" << std::endl;
//     std::string code;
//     std::cin >> code;
//     req.set_verify_code(code);
//     chat_im::PhoneRegisterRsp rsp;
//     brpc::Controller cntl;
//     chat_im::UserService_Stub stub(channel.get());
//     stub.PhoneRegister(&cntl, &req, &rsp, nullptr);
//     ASSERT_FALSE(cntl.Failed());
//     ASSERT_TRUE(rsp.success());
// }
TEST(用户子服务测试, 手机号登录) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    get_code();
    auto channel = _user_channels->choose(FLAGS_user_service);//获取通信信道
    ASSERT_TRUE(channel);

    chat_im::PhoneLoginReq req;
    req.set_request_id(chat_im::uuid());
    req.set_phone_number(user_info.phone());
    req.set_verify_code_id(code_id);
    std::cout << "手机号登录，输入验证码：" << std::endl;
    std::string code;
    std::cin >> code;
    req.set_verify_code(code);
    chat_im::PhoneLoginRsp rsp;
    brpc::Controller cntl;
    chat_im::UserService_Stub stub(channel.get());
    stub.PhoneLogin(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
    std::cout << "手机登录会话ID：" << rsp.login_session_id() << std::endl;
}
TEST(用户子服务测试, 手机号设置) {
    std::this_thread::sleep_for(std::chrono::seconds(10));
    get_code();
    auto channel = _user_channels->choose(FLAGS_user_service);//获取通信信道
    ASSERT_TRUE(channel);

    chat_im::SetUserPhoneNumberReq req;
    req.set_request_id(chat_im::uuid());
    std::cout << "手机号设置时，输入用户ID：" << std::endl;
    std::string user_id;
    std::cin >> user_id;
    req.set_user_id(user_id);
    req.set_phone_number("18106008871");
    req.set_phone_verify_code_id(code_id);
    std::cout << "手机号设置时，输入验证码：" << std::endl;
    std::string code;
    std::cin >> code;
    req.set_phone_verify_code(code);
    chat_im::SetUserPhoneNumberRsp rsp;
    brpc::Controller cntl;
    chat_im::UserService_Stub stub(channel.get());
    stub.SetUserPhoneNumber(&cntl, &req, &rsp, nullptr);
    ASSERT_FALSE(cntl.Failed());
    ASSERT_TRUE(rsp.success());
}

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    chat_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    //1. 先构造Rpc信道管理对象
    _user_channels = std::make_shared<chat_im::ServiceManager>();
    _user_channels->declared(FLAGS_user_service);
    auto put_cb = std::bind(&chat_im::ServiceManager::onServiceOnline, _user_channels.get(), std::placeholders::_1, std::placeholders::_2);
    auto del_cb = std::bind(&chat_im::ServiceManager::onServiceOffline, _user_channels.get(), std::placeholders::_1, std::placeholders::_2);
    
    //2. 构造服务发现对象
    chat_im::Discovery::ptr dclient = std::make_shared<chat_im::Discovery>(FLAGS_etcd_host, FLAGS_base_service, put_cb, del_cb);

    user_info.set_nickname("Yanzzp");
    user_info.set_user_id("e637-82348970-0001");
    user_info.set_description("Yanzzp");
    user_info.set_phone("18106008871");
    user_info.set_avatar("头像");
    testing::InitGoogleTest(&argc, argv);
    chat_im::DEBUG("开始测试！");
    return RUN_ALL_TESTS();
}