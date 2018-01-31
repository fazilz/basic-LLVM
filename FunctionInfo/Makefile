CXXFLAGS := $(shell llvm-config --cxxflags) -rdynamic -g -std=c++17

all: FunctionInfo.so

FunctionInfo.so: FunctionInfo.o
	$(CXX) -dylib -shared $^ -o $@

.PHONY: clean
clean:
	rm -f *.so *.o ./tests/*.ll ./tests/*.bc
