test :
	/usr/local/bin/protoc --cpp_out=../src ../src/*.proto -I../src
	g++ example.cpp ../src/*.cpp ../src/*.cc -I../src/ -llua -lprotobuf -ldl
	rm ../src/*.pb.h ../src/*.pb.cc
