#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
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
        // @TODO Please implement this.
        for(BasicBlock::iterator iter = BB.begin(); iter != M.end(); ++iter)
        {
        }

        return false;
    }

    bool runOnFunction(Function & F)
    {
        // @TODO Please implement this.
        std::string name = F.getName();
        ins_count[name] = 0;
        for(BasicBlock &bb : F)
        {
            ins_count[name] += bb.size();
            runOnBasicBlock(bb);
        }
        outs() << name <<" "<< F.arg_size() <<" 0 " << F.size() << ins_count[name] << '\n';
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
    runOnFunction(*iter);
}

// @TODO Please implement this.

return false;
}
};

char FunctionInfo::ID = 0; RegisterPass < FunctionInfo > X ("function-info", "CSCD70: Functions Information"); /* annoymous */ }
