all: test main

test: test.cc
	g++  $^ -o $@ -lfmt -lspdlog -lgflags -lcurl -lcrypto /usr/lib/x86_64-linux-gnu/libjsoncpp.so.1.8.4 -ljsoncpp -I../../code/server/third/include
main:main.cc
	g++ $^ -o $@ -lfmt -lspdlog -lgflags -lcurl -lcrypto /usr/lib/x86_64-linux-gnu/libjsoncpp.so.1.8.4 -ljsoncpp -I../../code/server/third/include