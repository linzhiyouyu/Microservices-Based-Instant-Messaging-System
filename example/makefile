all:discovery registry
discovery:discovery.cc
	@g++ -o $@ $^ -std=c++2a -lpthread -lspdlog -lfmt -lgflags -letcd-cpp-api -lcpprest
registry:registery.cc
	@g++ -o $@ $^ -std=c++2a -lpthread -lspdlog -lfmt -lgflags -letcd-cpp-api -lcpprest
.PHONY:clean
clean:
	@rm -rf discovery registry