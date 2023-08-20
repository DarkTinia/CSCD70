#include "DFA.h"
#include "DFA/Domain/Expression.h"
#include <cassert>

using namespace llvm;

AnalysisKey AvailExprs::Key;

void AvailExprs::initValue(Function &F) {
  for (auto &I : llvm::instructions(F)) {
    if (isa<BinaryOperator>(I)) {
      dfa::Expression Exp(*dyn_cast<BinaryOperator>(&I));
      if (DomainIdMap.find(Exp) == DomainIdMap.end()) {
        DomainVector.push_back(Exp);
        DomainIdMap.emplace(Exp, DomainVector.size() - 1);
      }
    }
  }
}

bool AvailExprs::transferFunc(const Instruction &Inst, const DomainVal_t &IDV,
                              DomainVal_t &ODV) {

  /// @todo(CSCD70) Please complete this method.

  DomainVal_t TmpODV = IDV;
  bool Changed = false;

  assert(IDV.size() == ODV.size());
  
  if (isa<BinaryOperator>(Inst)) {
    dfa::Expression Exp(*dyn_cast<BinaryOperator>(&Inst));
    auto Iter = DomainIdMap.find(Exp);
    if (Iter != DomainIdMap.end()) {
      TmpODV[Iter->second].Value = true;
    }
  }

  Changed |= !(TmpODV==ODV);
  ODV = TmpODV;

  // transferFuncDebug(Inst, IDV, ODV, Changed);

  return Changed;
}
