all: test main

test: test.cc
	g++ -std=c++17 $^ -o $@ -lalibabacloud-sdk-core -ljsoncpp

main: main.cc
	g++ -std=c++17 $^ -o $@ -lalibabacloud-sdk-core -lfmt -lspdlog -lgflags  -ljsoncpp