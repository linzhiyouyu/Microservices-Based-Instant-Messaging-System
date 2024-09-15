all: test main

test: test.cc
	g++ -std=c++17 $^ -o $@ -lfmt -lspdlog -lgflags -lcurl -lcrypto /usr/lib/x86_64-linux-gnu/libjsoncpp.so.1.8.4 -ljsoncpp
main:main.cc
	g++ -std=c++17 $^ -o $@ -lfmt -lspdlog -lgflags -lcurl -lcrypto /usr/lib/x86_64-linux-gnu/libjsoncpp.so.1.8.4 -ljsoncpp