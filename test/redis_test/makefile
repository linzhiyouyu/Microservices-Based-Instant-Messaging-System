main:main.cc
	@g++ -o $@ $^ -std=c++2a -lgflags -lhiredis -lredis++ -lpthread
.PHONY:clean
clean:
	@rm -rf main