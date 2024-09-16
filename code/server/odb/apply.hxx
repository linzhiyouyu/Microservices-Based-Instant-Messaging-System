#pragma once
#include <cstddef>
#include <odb/nullable.hxx>
#include <odb/core.hxx>

namespace chat_im {
    #pragma db object table("friend_apply")
    class FriendApply {
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