#include <ev.h>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <openssl/ssl.h>
#include <openssl/opensslv.h>
#include <functional>
void callback(AMQP::TcpChannel& channel, const AMQP::Message& msg, uint64_t deliveryTag, bool redelivered) {
    std::string _msg;
    _msg.assign(msg.body(), msg.bodySize());
    std::cout << _msg << std::endl;
    channel.ack(deliveryTag);
}

int main() {
    //1. 实例化底层网络通信框架的IO事件监控句柄
    auto *loop = EV_DEFAULT;
    AMQP::LibEvHandler handler(loop);
    //2. 实例化连接对象
    AMQP::Address address("amqp://root:123456@127.0.0.1:5672/");
    AMQP::TcpConnection connection(&handler, address);
    //3. 实例化信道对象
    AMQP::TcpChannel channel(&connection);
    //4. 声明交换机
    auto&& deferred = channel.declareExchange("test_exchange", AMQP::ExchangeType::direct);
    deferred.onError([&](const char* message)->void { std::cout << "声明交换机失败" << message << std::endl; exit(-1); });
    deferred.onSuccess([&]{ std::cout << "声明交换机成功" << std::endl; });
    //5. 声明队列
    auto&& queue = channel.declareQueue("test_queue");
    queue.onError([&](const char* message)->void { std::cout << "声明队列失败" << message << std::endl; exit(-1); });
    queue.onSuccess([&]{ std::cout << "声明队列成功" << std::endl; });
    //6. 针对交换机和队列进行绑定 
    auto&& binding = channel.bindQueue("test_exchange","test_queue", "test_queue_key");
    binding.onError([&](const char* message)->void { std::cout << "绑定交换机-队列失败" << message << std::endl; exit(-1); });
    binding.onSuccess([&]{ std::cout << "绑定交换机-队列成功" << std::endl; });

    auto&& consumer = channel.consume("test_queue", "consume-tag");
    consumer.onError([&](const char* msg) { std::cout << "订阅消息失败" << msg << std::endl; });
    auto msg_cb = std::bind(callback, std::ref(channel), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    consumer.onReceived(msg_cb);
    //7. 订阅队列消息 -- 设置消息处理回调
    ev_run(loop, 0);
    //8. 休眠、退出释放资源
    return 0;
}