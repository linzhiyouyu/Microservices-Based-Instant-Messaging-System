#pragma once
#include "mysql.hpp"
#include "chat_session_member.hxx"
#include "chat_session_member-odb.hxx"

namespace chat_im
{
    class ChatSessionMemeberTable
    {
    public:
        using ptr = std::shared_ptr<ChatSessionMemeberTable>;
        ChatSessionMemeberTable(const std::shared_ptr<odb::core::database> &db) : _db(db) {}
        // 单个会话成员的新增 --- ssid & uid
        bool append(ChatSessionMember &csm)
        {
            try
            {
                odb::transaction trans(_db->begin());
                _db->persist(csm);
                trans.commit();
            }
            catch (std::exception &e)
            {
                ERROR("新增单会话成员失败 {}-{}:{}！",
                      csm.session_id(), csm.user_id(), e.what());
                return false;
            }
            return true;
        }
        bool append(std::vector<ChatSessionMember> &csm_lists)
        {
            try
            {
                odb::transaction trans(_db->begin());
                for (auto &csm : csm_lists)
                {
                    _db->persist(csm);
                }
                trans.commit();
            }
            catch (std::exception &e)
            {
                ERROR("新增多会话成员失败 {}-{}:{}！",
                      csm_lists[0].session_id(), csm_lists.size(), e.what());
                return false;
            }
            return true;
        }
        // 删除指定会话中的指定成员 -- ssid & uid
        bool remove(ChatSessionMember &csm)
        {
            try
            {
                odb::transaction trans(_db->begin());
                typedef odb::query<ChatSessionMember> query;
                typedef odb::result<ChatSessionMember> result;
                query q(query::session_id == csm.session_id() &&
                        query::user_id == csm.user_id());
                _db->erase_query<ChatSessionMember>(q); 
                trans.commit();
            }
            catch (std::exception &e)
            {
                ERROR("删除单会话成员失败 {}-{}:{}！",
                      csm.session_id(), csm.user_id(), e.what());
                return false;
            }
            return true;
        }
        // 删除会话的所有成员信息
        bool remove(const std::string &ssid)
        {
            try
            {
                odb::transaction trans(_db->begin());
                typedef odb::query<ChatSessionMember> query;
                typedef odb::result<ChatSessionMember> result;
                query q(query::session_id == ssid);
                _db->erase_query<ChatSessionMember>(q);
                trans.commit();
            }
            catch (std::exception &e)
            {
                ERROR("删除会话所有成员失败 {}:{}！", ssid, e.what());
                return false;
            }
            return true;
        }
        std::vector<std::string> members(const std::string &ssid)
        {
            std::vector<std::string> res;
            try
            {
                odb::transaction trans(_db->begin());
                typedef odb::query<ChatSessionMember> query;
                typedef odb::result<ChatSessionMember> result;
                query q(query::session_id == ssid);
                result r(_db->query<ChatSessionMember>(q));
                for (result::iterator i(r.begin()); i != r.end(); ++i)
                {
                    res.push_back(i->user_id());
                }
                trans.commit();
            }
            catch (std::exception &e)
            {
                ERROR("获取会话成员失败:{}-{}！", ssid, e.what());
            }
            return res;
        }

    private:
        std::shared_ptr<odb::core::database> _db;
    };
}