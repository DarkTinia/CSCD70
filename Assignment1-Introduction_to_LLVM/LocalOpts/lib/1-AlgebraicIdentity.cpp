#include "LocalOpts.h"
#include <llvm-16/llvm/IR/Constant.h>
#include <llvm-16/llvm/IR/Constants.h>
#include <llvm-16/llvm/IR/InstrTypes.h>
#include <llvm-16/llvm/IR/Instruction.h>
#include <llvm-16/llvm/Support/Casting.h>
#include <llvm-16/llvm/Support/raw_ostream.h>

using namespace llvm;

PreservedAnalyses AlgebraicIdentityPass::run([[maybe_unused]] Function &F,
                                             FunctionAnalysisManager &) {
  /// @todo(CSCD70) Please complete this method.
  int Modify = 0;
  for (auto BB = F.begin(); BB != F.end(); BB++) {
    for (auto Ins = BB->begin(); Ins != BB->end();) {
      
      Instruction &Inscode = *Ins;
      
      outs() << "instruction is: " << Inscode << "\n";
      
      if (BinaryOperator *BO = dyn_cast<BinaryOperator>(
              &Inscode)) { // check if is binary operator
        Value *Operand0 = BO->getOperand(0);
        Value* Operand1 = BO->getOperand(1);

        switch (BO->getOpcode()) {
        
        case Instruction::Add:
        case Instruction::FAdd: // add 0
        {
          if (ConstantInt *C =
                  dyn_cast<ConstantInt>(Operand0)) { // check operand 1
            if (C->isZero()) {
              BO->replaceAllUsesWith(Operand1); // replace uses
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          } else if (ConstantInt *C = dyn_cast<ConstantInt>(
                         Operand0)) { // check operand 2
            if (C->isZero()) {
              BO->replaceAllUsesWith(Operand0);
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          }
          break;
        }
        case Instruction::Sub:
        case Instruction::FSub: // sub 0
        {
          outs() << "into sub\n";
          if (ConstantInt *C = dyn_cast<ConstantInt>(Operand1)) {
            if (C->isZero()) {
              BO->replaceAllUsesWith(Operand0);
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          }
          break;
        }
        case Instruction::Mul:
        case Instruction::FMul: //*0 *1
        {
          if (ConstantInt *C = dyn_cast<ConstantInt>(Operand0)) {
            if (C->isZero()) {
              BO->replaceAllUsesWith(Operand0);
              Ins = BO->eraseFromParent();
              Modify = 1;
            } else if (C->isOne()) {
              BO->replaceAllUsesWith(Operand1);
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          } else if (ConstantInt *C =
                         dyn_cast<ConstantInt>(Operand1)) {
            if (C->isZero()) {
              BO->replaceAllUsesWith(Operand1);
              Ins = BO->eraseFromParent();
              Modify = 1;
            } else if (C->isOne()) {
              BO->replaceAllUsesWith(Operand0);
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          }
          break;
        }
        case Instruction::UDiv:
        case Instruction::SDiv:
        case Instruction::FDiv: // /1
        {
          if (ConstantInt *C =
                  dyn_cast<ConstantInt>(Operand1)) { // check operand 1
            if (C->isOne()) {
              BO->replaceAllUsesWith(Operand0); // replace uses
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          }
          break;
        }
        case Instruction::URem: // rem 0   rem 1
        case Instruction::SRem:
        case Instruction::FRem: {
          if(ConstantInt *C = dyn_cast<ConstantInt>(Operand1)){
            if(C->isZero() || C->isOne()){
              BO->replaceAllUsesWith(ConstantInt::get(Operand0->getType(),0));
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          }
          break;
        }
        case Instruction::Shl:  // Shl 0
        case Instruction::LShr: // LShr 0
        case Instruction::AShr: // AShr 0
        {
          if (ConstantInt *C =
                  dyn_cast<ConstantInt>(Operand1)) { // check operand 1
            if (C->isZero()) {
              BO->replaceAllUsesWith(Operand0); // replace uses
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          }
          break;
        }
        case Instruction::And: // And 1
        {
          if (ConstantInt *C =
                  dyn_cast<ConstantInt>(Operand0)) { // check operand 1
            if (C->isOne()) {
              BO->replaceAllUsesWith(Operand1); // replace uses
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          } else if (ConstantInt *C = dyn_cast<ConstantInt>(
                         Operand1)) { // check operand 2
            if (C->isOne()) {
              BO->replaceAllUsesWith(Operand0);
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          }
          break;
        }
        case Instruction::Or: // Or 0
        {
          if (ConstantInt *C =
                  dyn_cast<ConstantInt>(Operand0)) { // check operand 1
            if (C->isZero()) {
              BO->replaceAllUsesWith(Operand1); // replace uses
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          } else if (ConstantInt *C = dyn_cast<ConstantInt>(
                         Operand1)) { // check operand 2
            if (C->isZero()) {
              BO->replaceAllUsesWith(Operand0);
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          }
          break;
        }
        case Instruction::Xor: // xor 0
        {
          if (ConstantInt *C =
                  dyn_cast<ConstantInt>(Operand0)) { // check operand 1
            if (C->isZero()) {
              BO->replaceAllUsesWith(Operand1); // replace uses
              Ins = BO->eraseFromParent();
              Modify = 1;
            }
          } else if (ConstantInt *C = dyn_cast<ConstantInt>(
                         Operand1)) { // check operand 2
            if (C->isZero()) {
              BO->replaceAllUsesWith(Operand0);
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

      if (!Modify) {
        Ins++;
      }
      Modify = 0;
    }
  }
  return PreservedAnalyses::none();
}
