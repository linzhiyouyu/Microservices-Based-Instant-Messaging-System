#include <sw/redis++/redis.h>
#include <gflags/gflags.h>
#include <iostream>
#include <chrono>
#include <vector>
DEFINE_string(ip, "127.0.0.1", "这是服务器的IP地址，格式：127.0.0.1");
DEFINE_int32(port, 6379, "这是服务端的端口，格式：6379");
DEFINE_int32(db, 0, "库的编号：默认0号");
DEFINE_bool(keep_alive, true, "是否进行长连接保活，默认为true");

void print(sw::redis::Redis& client) {
    auto user1 = client.get("会话ID1");
    if(user1)
        std::cout << *user1 << std::endl;
    auto user2 = client.get("会话ID2");
    if(user2)
        std::cout << *user2 << std::endl;
    auto user3 = client.get("会话ID3");
    if(user3)
        std::cout << *user3 << std::endl;
    auto user4 = client.get("会话ID4");
    if(user4)
        std::cout << *user4 << std::endl;
    auto user5 = client.get("会话ID5");
    if(user5)
        std::cout << *user5 << std::endl;
}

void add_string(sw::redis::Redis& client) {
    client.set("会话ID1", "用户ID1");
    client.set("会话ID2", "用户ID2");
    client.set("会话ID3", "用户ID3");
    client.set("会话ID4", "用户ID4");
    client.set("会话ID5", "用户ID5");

    client.del("会话ID3");
    client.set("会话ID5", "用户ID555");
    print(client);
}

void expire(sw::redis::Redis& client) {
    client.set("会话ID1", "用户ID111", std::chrono::milliseconds(1000));
    print(client);
    std::cout << "休眠2s" << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    print(client);
}

void list_test(sw::redis::Redis& client) {
    client.rpush("群聊1", "成员1");
    client.rpush("群聊1", "成员2");
    client.rpush("群聊1", "成员3");
    client.rpush("群聊1", "成员4");
    client.rpush("群聊1", "成员5");
    client.rpush("群聊1", "成员6");
    std::vector<std::string> ans;
    client.lrange("群聊1", 0, -1, std::back_inserter(ans));
    for(auto&& str : ans)
        std::cout << str << " ";
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    sw::redis::ConnectionOptions opts;
    opts.host = FLAGS_ip;
    opts.port = FLAGS_port;
    opts.db = FLAGS_db;
    opts.keep_alive = FLAGS_keep_alive;
    sw::redis::Redis client(opts);
    add_string(client);
    expire(client);
    list_test(client);
    return 0;
}