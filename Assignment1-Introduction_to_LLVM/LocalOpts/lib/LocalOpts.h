#pragma once // NOLINT(llvm-header-guard)
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>

class AlgebraicIdentityPass final
    : public llvm::PassInfoMixin<AlgebraicIdentityPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &,
                              llvm::FunctionAnalysisManager &);
}; // class AlgebraicIdentityPass

class StrengthReductionPass final
    : public llvm::PassInfoMixin<StrengthReductionPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &,
                              llvm::FunctionAnalysisManager &);
}; // class StrengthReductionPass

class MultiInstOptPass final : public llvm::PassInfoMixin<MultiInstOptPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &,
                              llvm::FunctionAnalysisManager &);
}; // class MultiInstOptPass
