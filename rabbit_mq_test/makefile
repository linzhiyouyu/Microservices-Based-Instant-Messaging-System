all: consume publish
consume: consume.cc
	@g++ -o $@ $^ -std=c++2a -lev -lamqpcpp
publish: publish.cc
	@g++ -o $@ $^ -std=c++2a -lev -lamqpcpp
.PHONY: clean
clean:
	@rm -rf consume publish