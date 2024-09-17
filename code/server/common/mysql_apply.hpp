#pragma once
#include "mysql.hpp"
#include "friend_apply.hxx"
#include "friend_apply-odb.hxx"

namespace chat_im {
    class FriendApplyTable {
    public:
        using ptr = std::shared_ptr<FriendApplyTable>;
        FriendApplyTable(std::shared_ptr<odb::core::database> db): _db(db) {}
        bool insert(FriendApply& ev) {
            try {
                odb::transaction trans(_db->begin());
                _db->persist(ev);
                trans.commit();
            }catch (std::exception &e) {
                ERROR("新增好友申请事件失败 {}-{}:{}!", ev.user_id(), ev.peer_id(), e.what());
                return false;
            }
            return true;
        }
        bool exists(const std::string &uid, const std::string &pid)
        {
            bool flag = false;
            try
            {
                typedef odb::query<FriendApply> query;
                typedef odb::result<FriendApply> result;
                odb::transaction trans(_db->begin());
                result r(_db->query<FriendApply>(query::user_id == uid && query::peer_id == pid));
                DEBUG("{} - {} 好友事件数量：{}", uid, pid, r.size());
                flag = !r.empty();
                trans.commit();
            }
            catch (std::exception &e)
            {
                ERROR("获取好友申请事件失败:{}-{}-{}！", uid, pid, e.what());
            }
            return flag;
        }
        bool remove(const std::string &uid, const std::string &pid) {
            try {
                odb::transaction trans(_db->begin());
                typedef odb::query<FriendApply> query;
                typedef odb::result<FriendApply> result;
                _db->erase_query<FriendApply>(query::user_id == uid && query::peer_id == pid);
                trans.commit();
            }catch (std::exception &e) {
                ERROR("删除好友申请事件失败 {}-{}:{}!", uid, pid, e.what());
                return false;
            }
            return true;
        }

        //获取当前指定用户的 所有好友申请者ID
        std::vector<std::string> applyUsers(const std::string &uid){
            std::vector<std::string> res;
            try {
                odb::transaction trans(_db->begin());
                using query =  odb::query<FriendApply>;
                using result =  odb::result<FriendApply>;
                //当前的uid是被申请者的用户ID
                result r(_db->query<FriendApply>(query::peer_id == uid));
                for (result::iterator i(r.begin()); i != r.end(); ++i) 
                    res.push_back(i->user_id());
                trans.commit();
            }catch (std::exception &e) {
                ERROR("通过用户{}的好友申请者失败:{}!", uid, e.what());
            }
            return res;
        }
    private:
        std::shared_ptr<odb::core::database> _db;
    };
}