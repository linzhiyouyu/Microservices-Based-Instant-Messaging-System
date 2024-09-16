all: brpc_client brpc_server
brpc_client: client.cc
	@g++ -o $@ $^ -std=c++2a -lbrpc -lleveldb -lgflags -lssl -lprotobuf -lcrypto -lgrpc -lpthread
brpc_server:server.cc
	@g++ -o $@ $^ -std=c++2a -lbrpc -lleveldb -lgflags -lssl -lprotobuf -lcrypto -lgrpc -lpthread
%.pb.cc : %.proto
	protoc --cpp_out ./ $<
.PHONY:clean
clean:
	@rm -rf brpc_client brpc_server