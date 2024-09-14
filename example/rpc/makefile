all:discovery registry
discovery:discovery.cc
	@g++ -o $@ $^ -std=c++2a -lpthread -lspdlog -lfmt -lgflags -letcd-cpp-api -lcpprest -lbrpc -lleveldb -lgflags -lssl -lprotobuf -lcrypto -lgrpc
registry:registery.cc
	@g++ -o $@ $^ -std=c++2a -lpthread -lspdlog -lfmt -lgflags -letcd-cpp-api -lcpprest -lbrpc -lleveldb -lgflags -lssl -lprotobuf -lcrypto -lgrpc 
.PHONY:clean
clean:
	@rm -rf discovery registry