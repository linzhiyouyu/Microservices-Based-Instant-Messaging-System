all:main test
main:main.cc
	@g++ -o $@ $^ -std=c++2a -lpthread -lcpr -lelasticlient
test:test.cc
	@g++ -o $@ $^ -std=c++2a -lpthread -lcpr -lelasticlient -ljsoncpp -lspdlog -lfmt -lgflags
.PHONY:clean
clean:
	@rm -rf main