#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/Bitcode/BitCodes.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Casting.h"
#include "llvm/ADT/APInt.h"
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
        for(BasicBlock::iterator iter = BB.begin(); iter != BB.end(); ++iter)
        {
            // check if the operand in a multiple of 2
            // switch to right or left shift.
            Instruction *ins = &(*iter);
            std::string name = ins->getOpcodeName();
            if(name == "Mul")
            {
                Value *op1 = ins->getOperand(0);
                Value *op2 = ins->getOperand(1);
                Instruction::BinaryOps shl = Instruction::BinaryOps::Shl;
                if (auto op1Cint = dyn_cast<ConstantInt>(op1))
                {
                    APInt op1_val = op1Cint->getValue();
                    if(op1_val.urem(2) == 0)
                    {
                        APInt divisor = (op1_val.udiv(2))-1;
                        Value *val = ConstantInt::get(op1->getType(), *(divisor.getRawData()));
                        Instruction *new_inst = BinaryOperator::Create(shl, op2, val);
                        ReplaceInstWithInst(ins, new_inst);
                        // ins->replaceAllUsesWith(new_inst);

                        strength_opt_count += 1;
                    }
                }
                else if (auto op2Cint = dyn_cast<ConstantInt>(op2))
                {
                    APInt op2_val = op2Cint->getValue();
                    if(op2_val.urem(2) == 0)
                    {
                        APInt divisor = (op2_val.udiv(2))-1;
                        Value *val = ConstantInt::get(op2->getType(), *(divisor.getRawData()));
                        Instruction *new_inst = BinaryOperator::Create(shl, op1, val);
                        ReplaceInstWithInst(ins, new_inst);
                        // ins->replaceAllUsesWith(new_inst);
                        strength_opt_count += 1;
                    }
                }
            }
            else if (name == "SDiv")
            {
                Instruction::BinaryOps ashr = Instruction::BinaryOps::AShr;
                Value *op2 = ins->getOperand(1);
                if (auto op2Cint = dyn_cast<ConstantInt>(op2))
                {
                    APInt op2_val = op2Cint->getValue();
                    if(op2_val.urem(2) == 0)
                    {
                        APInt divisor = (op2_val.udiv(2))-1;
                        Value *val = ConstantInt::get(op2->getType(), *(divisor.getRawData()));
                        Instruction *new_inst = BinaryOperator::Create(ashr, op2, val);
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
            outs() << "Transformations applied:\n"
                   << "Strength Reduction" << strength_opt_count << "\n";
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
            runOnFunction(*iter);
        }

        // @TODO Please implement this.

        return false;
    }
};

    char LocalOpts::ID = 0; RegisterPass < LocalOpts > X ("local-opts", "CSCD70: LocalOpts Information"); /* annoymous */ }
