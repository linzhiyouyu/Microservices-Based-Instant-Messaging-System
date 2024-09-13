#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
//1. 定义server类型
using server_t = websocketpp::server<websocketpp::config::asio>;
void onOpen(websocketpp::connection_hdl hdl) {
    std::cout << "长连接建立成功" << std::endl;
}
void onClose(websocketpp::connection_hdl hdl) {
    std::cout << "长连接断开" << std::endl;
}
void onMessage(server_t* server, websocketpp::connection_hdl hdl, server_t::message_ptr msg) {
    std::string body = msg->get_payload();
    std::cout << "收到消息: " << body << std::endl;
    auto conn = server->get_con_from_hdl(hdl);
    conn->send(body + "hello", websocketpp::frame::opcode::value::text);
}

int main() {
    
    //2. 实例化服务器对象
    server_t server;
    //3. 初始化日志输出 -- 关闭日志输出
    server.set_access_channels(websocketpp::log::alevel::none);
    //4. 初始化asio框架
    server.init_asio();
    server.set_reuse_addr(true);
    //5. 设置消息处理/连接握手成功/连接关闭回调函数
    server.set_open_handler(onOpen);
    server.set_close_handler(onClose);
    auto msg_hanler = std::bind(onMessage, &server, std::placeholders::_1, std::placeholders::_2);
    server.set_message_handler(msg_hanler); 
    //6. 设置监听端口
    server.listen(8081);
    server.start_accept();
    //7. 启动服务器
    server.run();
}