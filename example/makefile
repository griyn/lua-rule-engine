test :
	/usr/local/bin/protoc --cpp_out=../src ../src/*.proto -I../src
	/usr/local/bin/protoc --cpp_out=. *.proto
	g++ example.cpp *.pb.cc ../src/*.cpp ../src/*.cc -I../src/ -llua -lprotobuf -ldl
	rm ../src/*.pb.h ../src/*.pb.cc
	rm ./*.pb.h ./*.pb.cc
