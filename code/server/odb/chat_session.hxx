#pragma once
#include <cstddef>
#include <odb/nullable.hxx>
#include <odb/core.hxx>
#include "chat_session_member.hxx"

namespace chat_im {
    enum class ChatSessionType {
        SINGLE = 1,
        GROUP = 2
    };
    #pragma db object table("chat_session")
    class ChatSession {
    public:
        ChatSession() {}
        ChatSession(const std::string& ssid, const std::string& ssname, ChatSessionType sstype)
            :_chat_session_id(ssid), _chat_session_name(ssname), _chat_session_type(sstype) {}
        std::string chat_session_id() const { return _chat_session_id; }
        void chat_session_id(const std::string& ssid) { _chat_session_id = ssid; }
        std::string chat_session_name() const { return _chat_session_name; }
        void chat_session_name(const std::string& ssname) { _chat_session_name = ssname; }
        ChatSessionType chat_session_type() const { return _chat_session_type; }
        void chat_session_type(ChatSessionType sstype) { _chat_session_type = sstype; }
    private:
        friend class odb::access;
        #pragma db id auto
        unsigned long id;
        #pragma db type("varchar(64)") index unique
        std::string _chat_session_id;
        #pragma db type("varchar(64)") 
        std::string _chat_session_name;
        #pragma db type("tinyint")
        ChatSessionType _chat_session_type;   //1-单聊   2-群聊
    };

    #pragma db view object(ChatSession = css)\
                    object(ChatSessionMember = csm1 : css::_chat_session_id == csm1::_session_id)\
                    object(ChatSessionMember = csm2 : css::_chat_session_id == csm2::_session_id)\
                    query((?))
    struct SingleChatSession {
        #pragma db column(css::_chat_session_id)
        std::string chat_session_id;
        #pragma db column(csm2::_user_id)
        std::string friend_id;
    };

    #pragma db view object(ChatSession = css)\
                object(ChatSessionMember = csm : css::_chat_session_id == csm::_session_id)\
                query((?))
    struct GroupChatSession {
        #pragma db column(css::_chat_session_id)
        std::string chat_session_id;
        #pragma db column(css::_chat_session_name)
        std::string chat_session_name;
    };

}