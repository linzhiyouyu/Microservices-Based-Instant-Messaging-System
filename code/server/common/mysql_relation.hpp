#pragma once
#include "mysql.hpp"
#include "relation.hxx"
#include "relation-odb.hxx"

namespace chat_im {
    class RelationTable {
    public:
        using ptr = std::shared_ptr<RelationTable>;
        RelationTable(std::shared_ptr<odb::core::database> db): _db(db) {}
        //新增关系信息
        bool insert(const std::string& uid, const std::string& pid) {
            // 增加一组 {uid, pid} {pid, uid}
            try {
                Relation r1(uid, pid);
                Relation r2(pid, uid);
                odb::transaction trans(_db->begin());
                _db->persist(r1);
                _db->persist(r2);
                trans.commit();
            } catch(std::exception& e) {
                ERROR("新增好友关系失败{}-{}:{}", uid, pid, e.what());
                return false;
            }
            return true;
        }
        //移除关系信息
        bool remove(const std::string& uid, const std::string& pid) {
            try {
                odb::transaction trans(_db->begin());
                using query = odb::query<Relation>;
                using result = odb::result<Relation>;
                _db->erase<Relation>({query::user_id == uid && query::peer_id == pid});
                _db->erase<Relation>(query::user_id == pid && query::peer_id == uid);
                trans.commit();
            } catch(std::exception& e) {
                ERROR("删除好友关系失败{}-{}:{}!", uid, pid, e.what());
                return false;
            }
            return true;
        }
        //判断关系是否存在
        bool exists(const std::string &uid, const std::string &pid) {
            typedef odb::query<Relation> query;
            typedef odb::result<Relation> result;
            result r;
            bool flag = false;
            try {
                odb::transaction trans(_db->begin());
                r = _db->query<Relation>(query::user_id == uid && query::peer_id == pid);
                flag = !r.empty();
                trans.commit();
            }catch (std::exception &e) {
                ERROR("获取用户好友关系失败:{}-{}-{}!", uid, pid, e.what());
            }
            return flag;
        }

        //获取指定用户的好友ID
        std::vector<std::string> friends(const std::string &uid) {
            std::vector<std::string> res;
            try {
                odb::transaction trans(_db->begin());
                typedef odb::query<Relation> query;
                typedef odb::result<Relation> result;
                result r(_db->query<Relation>(query::user_id == uid));
                for (result::iterator i(r.begin()); i != r.end(); ++i) {
                    res.push_back(i->peer_id());
                }
                trans.commit();
            }catch (std::exception &e) {
                ERROR("通过用户-{}的所有好友ID失败:{}!", uid, e.what());
            }
            return res;
        }
    private:
        std::shared_ptr<odb::core::database> _db;
    };
}