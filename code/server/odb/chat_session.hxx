#pragma once
#include <cstddef>
#include <odb/nullable.hxx>
#include <odb/core.hxx>

namespace chat_im {
    #pragma db object table("chat_session")
    class ChatSession {
    public:
        ChatSession() {}
        ChatSession(const std::string& ssid, const std::string& ssname, unsigned char sstype)
            :_chat_session_id(ssid), _chat_session_name(ssname), _chat_session_type(sstype) {}
    private:
        friend class odb::access;
        #pragma db id auto
        unsigned long id;
        #pragma db type("varchar(64)") index unique
        std::string _chat_session_id;
        #pragma db type("varchar(64)") 
        std::string _chat_session_name;
        signed char _chat_session_type;   //1-单聊   2-群聊
    };

}