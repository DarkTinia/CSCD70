#include "LocalOpts.h"
#include <llvm-16/llvm/ADT/APInt.h>
#include <llvm-16/llvm/IR/BasicBlock.h>
#include <llvm-16/llvm/IR/Constants.h>
#include <llvm-16/llvm/IR/IRBuilder.h>
#include <llvm-16/llvm/IR/InstrTypes.h>
#include <llvm-16/llvm/IR/Instruction.h>
#include <llvm-16/llvm/IR/Value.h>

using namespace llvm;

int checkPowerOfTwo(int N) {

  if (N <= 0) {
    return -1; // no
  }

  if ((N & (N - 1)) != 0) {
    return -1; // no
  }

  int Power = 0;
  while (N > 1) {
    N /= 2;
    Power += 1;
  }

  return Power;
}

PreservedAnalyses StrengthReductionPass::run([[maybe_unused]] Function &F,
                                             FunctionAnalysisManager &) {

  /// @todo(CSCD70) Please complete this method.

  int Modify = 0;
  for (auto BB = F.begin(); BB != F.end(); BB++) {

    for (auto Ins = BB->begin(); Ins != BB->end();) {

      Instruction &Inscode = *Ins;
      outs() << "Instruction is: " << Inscode << "\n";
      if (BinaryOperator *BO = dyn_cast<BinaryOperator>(&Inscode)) {
        Value *Operand0 = BO->getOperand(0);
        Value *Operand1 = BO->getOperand(1);

        auto OP = BO->getOpcode();
        switch (OP) {

        case Instruction::Mul:
        case Instruction::FMul: {
          if (ConstantInt *C = dyn_cast<ConstantInt>(Operand0)) {
            int Power = checkPowerOfTwo(C->getValue().getLimitedValue());
            if (Power > 0) {
              BasicBlock &CurBB = *BB;
              IRBuilder<> Builder(&CurBB, Ins);
              Value *NewShl = Builder.CreateShl(Operand1, Power);
              BO->replaceAllUsesWith(NewShl);
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          } else if (ConstantInt *C = dyn_cast<ConstantInt>(Operand1)) {
            int Power = checkPowerOfTwo(C->getValue().getLimitedValue());
            if (Power > 0) {
              BasicBlock &CurBB = *BB;
              IRBuilder<> Builder(&CurBB, Ins);
              Value *NewShl = Builder.CreateShl(Operand0, Power);
              BO->replaceAllUsesWith(NewShl);
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          }
          break;
        }
        case Instruction::UDiv: { // unsigned
          if (ConstantInt *C = dyn_cast<ConstantInt>(Operand1)) {
            int Power = checkPowerOfTwo(C->getValue().getLimitedValue());
            if (Power > 0) {
              BasicBlock &CurBB = *BB;
              IRBuilder<> Builder(&CurBB, Ins);
              Value *NewLshr = Builder.CreateLShr(Operand0, Power);
              BO->replaceAllUsesWith(NewLshr);
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          }
          break;
        }
        case Instruction::SDiv: { // signed
          if (ConstantInt *C = dyn_cast<ConstantInt>(Operand1)) {
            int Power = checkPowerOfTwo(C->getValue().getLimitedValue());
            if (Power > 0) {
              BasicBlock &CurBB = *BB;
              IRBuilder<> Builder(&CurBB, Ins);
              Value *NewUshr = Builder.CreateAShr(Operand0, Power);
              BO->replaceAllUsesWith(NewUshr);
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          }
          break;
        }
        default: {
          break;
        }
        }
      }
      if (Modify == 0) {
        Ins++;
      }
      Modify = 0;
    }
  }

  return PreservedAnalyses::none();
}
