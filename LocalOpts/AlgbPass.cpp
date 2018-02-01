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
class LocalOpts : public ModulePass
{
private:
    int strength_opt_count = 0;

    bool strength_opt(BasicBlock &BB)
    {
        int old = strength_opt_count;
        // check if the operand in a multiple of 2
        // switch to right or left shift.
        for(BasicBlock::Iterator iter = BB.begin(); iter != BB.end(); ++iter)
        {
            Instruction *ins = &(*iter);
            if(auto call = dyn_cast<BinaryOperator>(ins))
            {
                std::string ins_name = ins->getOpcodeName();
                // check if instruction is a mult or a div
                int operand_1 = ins->getOperand(0);
                int operand_2 = ins->getOperand(1);
                if(ins_name.compare("mul"))
                {
                    if (operand_1%2 == 0)
                    {
                        int divisor = (operand_1 / 2)-1;
                        Instruction *new_inst = BinaryOperator::Create(Instruction::Shl, operand_2, divisor);
                        ins->replaceAllUsesWith(new_inst);
                        strength_opt_count += 1;
                    }
                    else if (operand_2%2 == 0)
                    {
                        int divisor = (operand_2 / 2)-1;
                        Instruction *new_inst = BinaryOperator::Create(Instruction::Shl, operand_1, divisor);
                        ins->replaceAllUsesWith(new_inst);
                        strength_opt_count += 1;
                    }
                }
                else if (ins_name.compare("div"))
                {
                    if (operand_2%2 == 0)
                    {
                        int divisor = (operand_2 / 2)-1;
                        Instruction *new_inst = BinaryOperator::Create(Instruction::AShr, operand_1, divisor);
                        ins->replaceAllUsesWith(new_inst);
                        strength_opt_count += 1;
                    }
                }
            }
        }
        if(old == strength_opt_count)
            return false;
        return true;
    }

    bool runOnBasicBlock(BasicBlock & BB)
    {
        if(strength_opt(BB))
        {
            return true;
        }
        return false;
    }

    bool runOnFunction(Function & F)
    {
        for(BasicBlock &bb : F)
        {
            // bb.size() == # of instructions in a basic block
            runOnBasicBlock(bb);
        }
        // F.size() == # of instructions in a Function
        return false;
    }

public:
    static char ID;

    LocalOpts() : ModulePass(ID)
    {}

    ~LocalOpts()
    {}

    // We don't modify the program, so we preserve all analysis.
    virtual void getAnalysisUsage(AnalysisUsage & AU) const
    {
        AU.setPreservesAll();
    }


    virtual bool runOnModule(Module & M)
    {

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

    char LocalOpts::ID = 0; RegisterPass < LocalOpts > X ("LocalOpts", "CSCD70: LocalOpts Information"); /* annoymous */ }
