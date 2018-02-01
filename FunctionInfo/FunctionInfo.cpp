#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Casting.h"
#include <map>
#include <string.h>

using namespace llvm;

namespace {
class FunctionInfo : public ModulePass
{
private:

    std::map<std::string, int> ins_count;
    std::map<std::string, int> calls_count;
    bool runOnBasicBlock(BasicBlock & BB)
    {
        // loop through every instruction and check if its a call
        // if it is then increment the correct datastructure.
        for(Instruction &ins : BB)
        {
            if(isa<CallInst>(ins))
            {
		auto call = cast<CallInst>(ins);
                std::string name = call.getCalledFunction()->getName();
                calls_count[name] += 1;
            }
        }

        return false;
    }

    bool runOnFunction(Function & F)
    {
        std::string name = F.getName();
        ins_count[name] = 0;
        calls_count[name] = 0;
        for(BasicBlock &bb : F)
        {
            // bb.size() == # of instructions in a basic block
            ins_count[name] += bb.size();
            runOnBasicBlock(bb);
        }
        // F.size() == # of instructions in a Function
        outs() << name << " "<< F.arg_size() << " " << calls_count[name] << " "
               << F.size() << " " << ins_count[name] << '\n';
        return false;
    }

public:
    static char ID;

    FunctionInfo() : ModulePass(ID)
    {}

    ~FunctionInfo()
    {}

    // We don't modify the program, so we preserve all analysis.
    virtual void getAnalysisUsage(AnalysisUsage & AU) const
    {
        AU.setPreservesAll();
    }


    virtual bool runOnModule(Module & M)
    {
        outs() << "Name #Args #Calls #Blocks #Insts" << "\n";

        for (Module::iterator iter = M.begin(); iter != M.end(); ++iter)
        {
            calls_count[(*iter).getName()] = 0;
            ins_count[(*iter).getName()] = 0;
            runOnFunction(*iter);
        }

        // @TODO Please implement this.

        return false;
    }
};

char FunctionInfo::ID = 0; RegisterPass < FunctionInfo > X ("function-info", "CSCD70: Functions Information"); /* annoymous */ }
