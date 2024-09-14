all: consume publish
consume: consume.cc
	@g++ -o $@ $^ -std=c++2a -lpthread -lfmt -lspdlog -lev -lamqpcpp -lgflags
publish: publish.cc
	@g++ -o $@ $^ -std=c++2a -lpthread -lfmt -lspdlog -lev -lamqpcpp -lgflags
.PHONY:clean
clean:
	@rm -rf consume publish