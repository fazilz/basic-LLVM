# @TODO Please complete this file.
CXXFLAGS := $(shell llvm-config --cxxflags) -rdynamic -g -std=c++17

all: AlgbPass.so

AlgbPass.so: AlgbPass.o
	$(CXX) -dylib -shard $^ -o $@

clean:
	rm -rf *.so *.o ./tests/*.ll ./tests/*.bc
