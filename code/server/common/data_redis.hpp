#include <sw/redis++/redis.h>
#include <iostream>

namespace chat_im
{
    // redis工厂管理，用于创建redis客户端
    class RedisClientFactory
    {
    public:
        static std::shared_ptr<sw::redis::Redis> create(
            const std::string &host,
            int port,
            int db,
            bool keep_alive,
            const std::string &password = "")
        {
            sw::redis::ConnectionOptions opts;
            opts.host = host;
            opts.port = port;
            opts.db = db;
            opts.keep_alive = keep_alive;
            if (!password.empty())
            {
                opts.password = password;
            }
            auto res = std::make_shared<sw::redis::Redis>(opts);
            return res;
        }
    };

    // 用户会话，用于存储用户登录状态
    class Session
    {
    public:
        using ptr = std::shared_ptr<Session>;
        Session(const std::shared_ptr<sw::redis::Redis> &redis_client) : _redis_client(redis_client) {}
        void append(const std::string &ssid, const std::string &uid)
        {
            _redis_client->set(ssid, uid);
        }
        void remove(const std::string &ssid)
        {
            _redis_client->del(ssid);
        }
        sw::redis::OptionalString uid(const std::string &ssid)
        {
            return _redis_client->get(ssid);
        }

    private:
        std::shared_ptr<sw::redis::Redis> _redis_client;
    };

    // 登录状态，用于存储用户登录状态
    class Status
    {
    public:
        using ptr = std::shared_ptr<Status>;
        Status(const std::shared_ptr<sw::redis::Redis> &redis_client) : _redis_client(redis_client) {}
        void append(const std::string &uid)
        {
            _redis_client->set(uid, "");
        }
        void remove(const std::string &uid)
        {
            _redis_client->del(uid);
        }
        bool exists(const std::string &uid)
        {
            auto res = _redis_client->get(uid);
            if (res)
                return true;
            return false;
        }

    private:
        std::shared_ptr<sw::redis::Redis> _redis_client;
    };

    // 验证码管理，验证码和用户id的映射
    class Codes
    {
    public:
        using ptr = std::shared_ptr<Codes>;
        Codes(const std::shared_ptr<sw::redis::Redis> &redis_client) : _redis_client(redis_client) {}
        void append(const std::string &cid, const std::string &code,
                    const std::chrono::milliseconds &t = std::chrono::milliseconds(300000))
        {
            _redis_client->set(cid, code, t);
        }
        void remove(const std::string &cid)
        {
            _redis_client->del(cid);
        }
        sw::redis::OptionalString code(const std::string &cid)
        {
            return _redis_client->get(cid);
        }

    private:
        // redis客户端指针
        std::shared_ptr<sw::redis::Redis> _redis_client;
    };
}