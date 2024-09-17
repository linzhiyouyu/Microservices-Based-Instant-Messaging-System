#!/bin/bash
declare depends
get_depends() {
    depends=$( ldd $1 | awk '{if (match($3,"/")){ print $3}}' )
    mkdir $2
    cp -Lr $depends $2
}

get_depends ./gateway/build/gateway_server ./gateway/depends
get_depends ./user/build/user_server ./user/depends
get_depends ./file/build/file_server ./file/depends
get_depends ./friend/build/friend_server ./friend/depends
get_depends ./speech/build/speech_server ./speech/depends
get_depends ./message/build/message_server ./message/depends
get_depends ./transmit/build/transmit_server ./transmit/depends

get_depends nc ./gateway/depends
get_depends nc ./user/depends
get_depends nc ./file/depends
get_depends nc ./friend/depends
get_depends nc ./speech/depends
get_depends nc ./message/depends
get_depends nc ./transmit/depends

cp /bin/nc ./gateway/
cp /bin/nc ./user/
cp /bin/nc ./file/
cp /bin/nc ./friend/
cp /bin/nc ./speech/
cp /bin/nc ./message/
cp /bin/nc ./transmit/

