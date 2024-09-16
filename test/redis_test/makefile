main : main.cc
	g++ -std=c++17 $^ -o $@ -lhiredis -lredis++ -lgflags 
.PHONY:clean
clean:
	@rm -rf main