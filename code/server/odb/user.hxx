#pragma once
#include <string>
#include <cstddef>
#include <odb/nullable.hxx>
#include <odb/core.hxx>

namespace chat_im
{
#pragma db object table("user")
    class User
    {
    public:
        User() {}
        // 用户名--新增用户 -- 用户ID, 昵称，密码
        User(const std::string &uid, const std::string &nickname, const std::string &password) : _user_id(uid), _nickname(nickname), _password(password) {}
        // 手机号--新增用户 -- 用户ID, 手机号, 随机昵称
        User(const std::string &uid, const std::string &phone) : _user_id(uid), _nickname(uid), _phone(phone) {}

        void user_id(const std::string &val) { _user_id = val; }
        std::string user_id() { return _user_id; }

        std::string nickname()
        {
            if (_nickname)
                return *_nickname;
            return std::string();
        }
        void nickname(const std::string &val) { _nickname = val; }

        std::string description()
        {
            if (!_description)
                return std::string();
            return *_description;
        }
        void description(const std::string &val) { _description = val; }

        std::string password()
        {
            if (!_password)
                return std::string();
            return *_password;
        }
        void password(const std::string &val) { _password = val; }

        std::string phone()
        {
            if (!_phone)
                return std::string();
            return *_phone;
        }
        void phone(const std::string &val) { _phone = val; }

        std::string avatar_id()
        {
            if (!_avatar_id)
                return std::string();
            return *_avatar_id;
        }
        void avatar_id(const std::string &val) { _avatar_id = val; }

    private:
        friend class odb::access;
#pragma db id auto
        unsigned long _id;
#pragma db type("varchar(64)") index unique
        std::string _user_id;
#pragma db type("varchar(64)") index unique
        odb::nullable<std::string> _nickname;    // 用户昵称-不一定存在
        odb::nullable<std::string> _description; // 用户签名 - 不一定存在
#pragma db type("varchar(64)")
        odb::nullable<std::string> _password; // 用户密码 - 不一定存在
#pragma db type("varchar(64)") index unique
        odb::nullable<std::string> _phone; // 用户手机号 - 不一定存在
#pragma db type("varchar(64)")
        odb::nullable<std::string> _avatar_id; // 用户头像文件ID - 不一定存在
    };
}
// odb -d mysql --std c++11 --generate-query --generate-schema --profile boost/date-time person.hxx