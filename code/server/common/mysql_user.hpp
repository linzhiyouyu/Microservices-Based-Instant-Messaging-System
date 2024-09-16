#include "mysql.hpp"
// #include "user.hxx"
// #include "user-odb.hxx"
#include "../odb/chat_session_member.hxx"
#include "../transmit/test/mysql_test/chat_session_member-odb.hxx"

namespace chat_im
{
// class UserTable
// {
// private:
//     std::shared_ptr<odb::core::database> _db;


public:
    UserTable();
    UserTable(const std ::shared_ptr<odb::core::database> &db) : _db(db) {};
    ~UserTable() = default;

    bool insert(const std::shared_ptr<User> &user)
    {
        try
        {
            odb::transaction trans(_db->begin());
            _db->persist(*user);
            trans.commit();
        }
        catch (std::exception &e)
        {
            ERROR("新增用户失败 {}:{}！", user->nickname(), e.what());
            return false;
        }
        return true;
    }
    bool update(const std::shared_ptr<User> &user)
    {
        try
        {
            odb::transaction trans(_db->begin());
            _db->update(*user);
            trans.commit();
        }
        catch (std::exception &e)
        {
            ERROR("更新用户失败 {}:{}！", user->nickname(), e.what());
            return false;
        }
        return true;
    }
    std::shared_ptr<User> select_by_nickname(const std::string &nickname)
    {
        std::shared_ptr<User> res;
        try
        {
            odb::transaction trans(_db->begin());
            typedef odb::query<User> query;
            typedef odb::result<User> result;
            res.reset(_db->query_one<User>(query::nickname == nickname));
            trans.commit();
        }
        catch (std::exception &e)
        {
            ERROR("通过昵称查询用户失败 {}:{}！", nickname, e.what());
        }
        return res;
    }
    std::shared_ptr<User> select_by_phone(const std::string &phone)
    {
        std::shared_ptr<User> res;
        try
        {
            odb::transaction trans(_db->begin());
            typedef odb::query<User> query;
            typedef odb::result<User> result;
            res.reset(_db->query_one<User>(query::phone == phone));
            trans.commit();
        }
        catch (std::exception &e)
        {
            ERROR("通过手机号查询用户失败 {}:{}！", phone, e.what());
        }
        return res;
    }
    std::shared_ptr<User> select_by_id(const std::string &user_id)
    {
        std::shared_ptr<User> res;
        try
        {
            odb::transaction trans(_db->begin());
            typedef odb::query<User> query;
            typedef odb::result<User> result;
            res.reset(_db->query_one<User>(query::user_id == user_id));
            trans.commit();
        }
        catch (std::exception &e)
        {
            ERROR("通过用户ID查询用户失败 {}:{}！", user_id, e.what());
        }
        return res;
    }
    std::vector<User> select_multi_users(const std::vector<std::string> &id_list)
    {
        // select * from user where id in ('id1', 'id2', ...)
        if (id_list.empty())
        {
            return std::vector<User>();
        }
    private:
        std::shared_ptr<odb::core::database> _db;
    };


} // namespace chat_im