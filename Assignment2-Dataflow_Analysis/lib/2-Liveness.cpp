#include "DFA.h"
#include "DFA/Domain/Variable.h"
#include <cassert>
#include <llvm-16/llvm/IR/Argument.h>
#include <llvm-16/llvm/IR/Instruction.h>
#include <llvm-16/llvm/Support/raw_ostream.h>

/// @todo(CSCD70) Please complete this file.

using namespace llvm;

AnalysisKey Liveness::Key;

void Liveness::initValue(llvm::Function &F) {
  for (auto &Para : F.args()) {
    dfa::Variable Var(&Para);
    DomainVector.push_back(Var);
    DomainIdMap.emplace(Var, DomainVector.size() - 1);
  }
  for (auto &Ins : llvm::instructions(F)) {
    dfa::Variable Var(&Ins);
    if (Ins.getNumUses()) {
      llvm::outs() << "Ins is: " << Ins << "\n";
      DomainVector.push_back(Var);
      DomainIdMap.emplace(Var, DomainVector.size() - 1);
    }
  }
}

bool Liveness::transferFunc(const llvm::Instruction &Inst,
                            const DomainVal_t &IDV, DomainVal_t &ODV) {
  assert(IDV.size() == ODV.size());
  bool Changed = false;
  DomainVal_t TmpODV = IDV;
  for (const auto *Iter = Inst.op_begin(); Iter != Inst.op_end(); Iter++) {
    Value *V = *Iter;
    if (isa<Instruction>(V) || isa<Argument>(V)) {
      dfa::Variable Value(V);
      TmpODV.at(DomainIdMap.at(Value)).Value = true;
    }
  }
  if(Inst.getNumUses()){
    dfa::Variable Value(&Inst);
    TmpODV.at(DomainIdMap.at(Value)).Value = false;
  }

  Changed = !(TmpODV == ODV);
  ODV = TmpODV;

  transferFuncDebug(Inst, IDV, ODV, Changed);

  return Changed;
}
