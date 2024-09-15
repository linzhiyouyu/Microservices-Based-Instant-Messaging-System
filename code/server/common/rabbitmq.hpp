#pragma once
#include <ev.h>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <openssl/ssl.h>
#include <openssl/opensslv.h>
#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include "logger.hpp"
namespace chat_im {
class MQClient {
public:
    using message_cb = std::function<void(const char*, size_t)>;
    MQClient(const std::string &user,
            const std::string& pswd,
            const std::string& host) {
        _loop = EV_DEFAULT;
        _handler = std::make_unique<AMQP::LibEvHandler>(_loop);
        std::string url = "amqp://" + user + ":" + pswd + "@" + host + "/";
        AMQP::Address address(url);
        _connection = std::make_unique<AMQP::TcpConnection>(_handler.get(), address);
        _channel = std::make_unique<AMQP::TcpChannel>(_connection.get());
        _loop_thread = std::thread([&] { ev_run(_loop, 0); });
    }
    ~MQClient() {
        struct ev_async async_watcher;
        ev_async_init(&async_watcher, watcher_cb);
        ev_async_start(_loop, &async_watcher);
        ev_async_send(_loop, &async_watcher);
        _loop_thread.join();
        _loop = nullptr;
    }
    void declareComponents(const std::string& exchangename, 
                            const std::string& queue,
                            const std::string& routing_key= "routing_key",
                            AMQP::ExchangeType exchange_type = AMQP::ExchangeType::direct) {
        //4. 声明交换机
        auto&& deferred = _channel->declareExchange(exchangename, exchange_type);
        deferred.onError([&](const char* message)->void { ERROR("声明交换机失败：{}", message); exit(-1); });
        deferred.onSuccess([&]{ DEBUG("{}: 交换机创建成功", exchangename); });
        //5. 声明队列
        auto&& q = _channel->declareQueue(queue);
        q.onError([&](const char* message)->void { ERROR("声明队列失败：{}", message); exit(-1); });
        q.onSuccess([&]{ DEBUG("{}: 队列创建成功", queue); });
        //6. 针对交换机和队列进行绑定 
        auto&& binding = _channel->bindQueue(exchangename, queue, routing_key);
        binding.onError([&](const char* message)->void { ERROR("{}:{} 绑定失败：{}", exchangename, queue, message); exit(-1); });
        binding.onSuccess([&]{ DEBUG("{}:{}绑定成功", exchangename, queue); });
    }
    bool publish(const std::string& exchange,
                const std::string& msg,
                const std::string& routing_key = "routing_key") {
        if(_channel->publish(exchange, routing_key, msg) == false) {
            ERROR("{}发布消息失败", exchange);
            return false;
        }
        return true;
    }
    void consume(const std::string& queue, const message_cb& cb) {
        auto&& consumer = _channel->consume(queue, "consume-tag");
        consumer.onReceived([&](const AMQP::Message& msg, uint64_t deliveryTag, bool redelivered) {
            cb(msg.body(), msg.bodySize());
            _channel->ack(redelivered);
        });
        consumer.onError([&](const char* msg) { ERROR("订阅 {} 队列消息失败: {}", queue, msg)});
    }
private:
    static void watcher_cb(struct ev_loop* loop, ev_async* watcher, int32_t revents) {
        ev_break(loop, EVBREAK_ALL);
    }
private:
    struct ev_loop* _loop;
    std::unique_ptr<AMQP::LibEvHandler> _handler;
    std::unique_ptr<AMQP::TcpConnection> _connection;
    std::unique_ptr<AMQP::TcpChannel> _channel;
    std::thread _loop_thread;
};
}