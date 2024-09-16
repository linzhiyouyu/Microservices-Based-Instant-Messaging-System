#pragma once
#include <cstddef>
#include <odb/nullable.hxx>
#include <odb/core.hxx>

namespace chat_im {
    #pragma db object table("friend_apply")
    class FriendApply {
        FriendApply() {}
        FriendApply(const std::string& eid, const std::string& uid, const std::string& pid)
            :_user_id(uid), _peer_id(pid), _event_id(eid) {}
        std::string event_id() const { return _event_id; }
        void event_id(const std::string& eid) { _event_id = eid; }
        std::string user_id() const { return _user_id; }
        void user_id(const std::string& uid) { _user_id = uid; }
        std::string _peer_id() const { return _peer_id; }
        void peer_id(const std::string& pid) { _peer_id = pid; }
    private:
        friend class odb::access;
        #pragma db id auto
        unsigned long _id;
        #pragma db type("varchar(64)") index
        std::string _event_id;
        #pragma db type("varchar(64)") index
        std::string _user_id;
        #pragma db type("varchar(64)") index
        std::string _peer_id;
    };
}