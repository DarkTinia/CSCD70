#include "LocalOpts.h"
#include <llvm-16/llvm/ADT/APInt.h>
#include <llvm-16/llvm/IR/BasicBlock.h>
#include <llvm-16/llvm/IR/Constants.h>
#include <llvm-16/llvm/IR/IRBuilder.h>
#include <llvm-16/llvm/IR/InstrTypes.h>
#include <llvm-16/llvm/IR/Instruction.h>
#include <llvm-16/llvm/IR/Value.h>

using namespace llvm;

bool isOpVar(BinaryOperator *BO, Value *Var, ConstantInt *Con, bool &IsLeft) {
  bool Flag = false;
  Value *Operand0 = BO->getOperand(0);
  Value *Operand1 = BO->getOperand(1);

  if (ConstantInt *C = dyn_cast<ConstantInt>(Operand0)) {
    if (ConstantInt *C1 = dyn_cast<ConstantInt>(Operand1)) {
      Flag = false;
    } else {
      Var = Operand1;
      Con = C;
      IsLeft = true;
      Flag = true;
    }
  } else if (ConstantInt *C = dyn_cast<ConstantInt>(Operand1)) {
    Var = Operand0;
    Con = C;
    IsLeft = false;
    Flag = true;
  }

  return Flag;
}

PreservedAnalyses MultiInstOptPass::run([[maybe_unused]] Function &F,
                                        FunctionAnalysisManager &) {

  /// @todo(CSCD70) Please complete this method.
  int Modify = 0;
  for (auto BB = F.begin(); BB != F.end(); BB++) {
    for (auto Ins = BB->begin(); Ins != BB->end(); Ins++) {
      Instruction &Inscode = *Ins;
      if (BinaryOperator *BO = dyn_cast<BinaryOperator>(&Inscode)) {
        auto OP = BO->getOpcode();
        Value *Operand0 = BO->getOperand(0);
        Value *Operand1 = BO->getOperand(1);

        ConstantInt *Con0;   // Constant for child
        ConstantInt *Con1;   // Constant for parent
        Value *Var;          // Variable
        bool IsLeft = false; // just for Sub and Div

        switch (OP) {

        case Instruction::Add: {
          outs() << "into add\n";
          if (isOpVar(BO, Var, Con0, IsLeft)) {
            outs() << "into 1\n";
            if (BinaryOperator *PreBO = dyn_cast<BinaryOperator>(Var)) {
              outs() << "into 2\n";
              auto PreOP = PreBO->getOpcode();
              if (PreOP == BinaryOperator::Add) {
                outs() << "into 3\n";
                if (isOpVar(PreBO, Var, Con1, IsLeft)) {
                  outs() << "into 4\n";
                  BO->setOperand(0, Var);
                  int Newnum = Con0->getValue().getLimitedValue() +
                               Con1->getValue().getLimitedValue();
                  BO->setOperand(
                      1, ConstantInt::getSigned(Con1->getType(), Newnum));
                }
              } else if (PreOP == BinaryOperator::Sub) {
              }
            }
          }
        }
        case Instruction::FAdd:
        case Instruction::Sub:
        case Instruction::FSub:
        case Instruction::Mul:
        case Instruction::FMul:
        case Instruction::UDiv:
        case Instruction::SDiv:
        case Instruction::FDiv:
        case Instruction::URem:
        case Instruction::SRem:
        case Instruction::FRem:
        case Instruction::Shl:
        case Instruction::LShr:
        case Instruction::AShr:
        case Instruction::And:
        case Instruction::Or:
        case Instruction::Xor:
        case Instruction::BinaryOpsEnd:
          break;
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
