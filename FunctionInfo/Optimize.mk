OPTIMIZER := ./FunctionInfo.so

OPT_PASSES := -function-info

CXXFLAGS := $(shell llvm-config --cxxflags) -rdynamic -g -std=c++17

OPT_OBJs := $(subst .cpp,.o,$(wildcard *.cpp))

TEST_SRCs := $(basename $(notdir $(wildcard ./tests/*.c)))
TEST_RAW_LLs := $(subst .c,.ll,$(wildcard ./tests/*.c))
TEST_OPT_LLs := $(addprefix ./tests/,$(addsuffix -opt.ll,$(TEST_SRCs)))

all: $(TEST_OPT_LLs) $(TEST_RAW_LLs) 

./tests/%.ll: ./tests/%.bc
	llvm-dis $< -o=$@

./tests/%-opt.bc: ./tests/%.bc $(OPTIMIZER)
	opt -load $(OPTIMIZER) $(OPT_PASSES) $< -o $@

./tests/%.bc: ./tests/%.c
	clang -O -emit-llvm -c $< -o $@

%.so: $(OPT_OBJs)
	$(CXX) -dylib -shared $^ -o $@

.PHONY: clean
clean:
	rm -f ./tests/*.ll
