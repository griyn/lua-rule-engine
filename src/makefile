test :
    /usr/local/bin/protoc --cpp_out=. *.proto
    g++ *.cpp *.cc -llua -lprotobuf -ldl
    rm *.pb.h *.pb.cc
