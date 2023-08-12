#include "LocalOpts.h"
#include <cstddef>
#include <llvm-16/llvm/ADT/APInt.h>
#include <llvm-16/llvm/IR/BasicBlock.h>
#include <llvm-16/llvm/IR/Constant.h>
#include <llvm-16/llvm/IR/Constants.h>
#include <llvm-16/llvm/IR/IRBuilder.h>
#include <llvm-16/llvm/IR/InstrTypes.h>
#include <llvm-16/llvm/IR/Instruction.h>
#include <llvm-16/llvm/IR/Value.h>
#include <vector>

using namespace llvm;

bool insCon(const Instruction &I) {
  return isa<Instruction>(I.getOperand(0)) && isa<ConstantInt>(I.getOperand(1));
}

bool conIns(const Instruction &I) {
  return isa<ConstantInt>(I.getOperand(0)) && isa<Instruction>(I.getOperand(1));
}

PreservedAnalyses MultiInstOptPass::run([[maybe_unused]] Function &F,
                                        FunctionAnalysisManager &) {

  // @todo(CSCD70) Please complete this method.
  int Modify = 0;
  std::vector<Instruction *> DeleteInst;

  for (auto BB = F.begin(); BB != F.end(); BB++) {
    for (auto Ins = BB->begin(); Ins != BB->end(); Ins++) {
      Instruction &CurInscode = *Ins;
      outs() << "current instruction is: " << CurInscode << "\n";
      if (CurInscode.isBinaryOp()) {
        Value *Operand0 = CurInscode.getOperand(0);
        Value *Operand1 = CurInscode.getOperand(1);

        Instruction *CurIns = nullptr;
        ConstantInt *CurCon = nullptr;
        int CurInsCon = -1;
        if (insCon(CurInscode)) {
          CurInsCon = 1;
          CurIns = dyn_cast<Instruction>(Operand0);
          CurCon = dyn_cast<ConstantInt>(Operand1);
        } else if (conIns(CurInscode)) {
          CurInsCon = 0;
          CurIns = dyn_cast<Instruction>(Operand1);
          CurCon = dyn_cast<ConstantInt>(Operand0);
        } else {
          continue;
        }
        int PreInsCon = -1;
        ConstantInt *PreCon = nullptr;
        Instruction *PreIns = nullptr;
        if (CurIns->isBinaryOp()) {
          if (insCon(*CurIns)) {
            PreInsCon = 1;
            PreIns = dyn_cast<Instruction>(CurIns->getOperand(0));
            PreCon = dyn_cast<ConstantInt>(CurIns->getOperand(1));
          } else if (conIns(*CurIns)) {
            PreInsCon = 0;
            PreIns = dyn_cast<Instruction>(CurIns->getOperand(1));
            PreCon = dyn_cast<ConstantInt>(CurIns->getOperand(0));
          } else {
            continue;
          }
        }
        switch (CurInscode.getOpcode()) {
        case Instruction::Add: {
          if (CurIns->getOpcode() == Instruction::Sub) {
            if (PreInsCon &&
                (PreCon->getSExtValue() == CurCon->getSExtValue())) {
              CurInscode.replaceAllUsesWith(PreIns);
              DeleteInst.push_back(&CurInscode);
            }
          }
          break;
        }
        case Instruction::Sub: {
          if (CurIns->getOpcode() == Instruction::Add) {
            if (CurInsCon &&
                (PreCon->getSExtValue() == CurCon->getSExtValue())) {
              CurInscode.replaceAllUsesWith(PreIns);
              DeleteInst.push_back(&CurInscode);
            }
          }
          break;
        }
        case Instruction::Mul: {
          if (CurIns->getOpcode() == Instruction::SDiv) {
            if (CurInsCon &&
                (PreCon->getSExtValue() == CurCon->getSExtValue())) {
              CurInscode.replaceAllUsesWith(PreIns);
              DeleteInst.push_back(&CurInscode);
            }
          }
          break;
        }
        case Instruction::SDiv: {
          if (CurIns->getOpcode() == Instruction::Mul) {
            if (PreCon->getSExtValue() == CurCon->getSExtValue()) {
              CurInscode.replaceAllUsesWith(PreIns);
              DeleteInst.push_back(&CurInscode);
            }
          }
          break;
        }
        }
      }
    }
  }

  for (auto &I : DeleteInst) {
    I->eraseFromParent();
  }

  return PreservedAnalyses::none();
}
