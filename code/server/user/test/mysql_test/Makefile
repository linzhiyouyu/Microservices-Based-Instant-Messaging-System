main : main.cc user-odb.cxx
	g++ -std=c++17 $^ -o $@  -I/usr/include/mysql -I../../../odb/ -I./ -lodb-mysql  -lodb -lodb-boost -lfmt -lspdlog -lgflags