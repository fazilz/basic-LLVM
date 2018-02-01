# @TODO Please complete this file.
OPTIMIZER := ./LocalOpts.so

CXXFLAGS := $(shell llvm-config --cxxflags) -rdynamic -g -std=c++17


OPT_OBJs := $(subst .cpp,.o,$(wildcard *.cpp))

TEST_SRCs := $(basename $(notdir $(wildcard ./tests/*.c)))
TEST_RAW_LLs := $(subst .c,.ll,$(wildcard ./tests/*.c))
TEST_OPT_LLs := $(addprefix ./tests/,$(addsuffix -opt.ll,$(TEST_SRCs)))

all: LocalOpts.so $(TEST_OPT_LLs) $(TEST_RAW_LLs)

./tests/%.ll: ./tests/%.bc
	llvm-dis $< -o=$@

./tests/%-opt.bc: ./tests/%.bc $(OPTIMIZER)
	opt -load $(OPTIMIZER) $(OPT_PASSES) $< -o $@

./tests/%.bc: ./tests/%.c
	clang -O -emit-llvm -c $< -o $@

%.so: $(OPT_OBJs)
	$(CXX) -dylib -shared $^ -o $@

# LocalOpts.so: LocalOpts.o
# 	$(CXX) -dylib -shard $^ -o $@

clean:
	rm -rf *.so *.o ./tests/*.ll ./tests/*.bc
