#!/bin/bash
# wait_for 127.0.0.1 3306
wiat_for() {
    while ! nc -z $1 $2; 
    do 
    echo "waiting for $1:$2";
    sleep 1;
    done
    echo "$1:$2 检测完毕"
}

declare DEPENDS_PORT
declare DEOENDS_HOST
declare CMD

while getopts "h:p:c:" arg
do
    case $arg in
        h)
            DEOENDS_HOST=$OPTARG ;;
        p)
            DEPENDS_PORT=$OPTARG ;;
        c)
            CMD=$OPTARG ;;
    esac
done

for var in ${DEPENDS_PORT//,/ }
do
    wiat_for $DEOENDS_HOST $var
done

echo "检测完毕"
eval $CMD