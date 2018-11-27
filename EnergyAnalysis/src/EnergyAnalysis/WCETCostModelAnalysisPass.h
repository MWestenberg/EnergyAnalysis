#pragma once
#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Analysis/TargetTransformInfoImpl.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/LegacyPassManagers.h"
#include "llvm/IR/Dominators.h"


class WCETCostModelAnalysisPass : public llvm::FunctionPass, public llvm::PMDataManager, public llvm::PMTopLevelManager
{

public:
	static char ID; // Class identification, replacement for typeinfo
	WCETCostModelAnalysisPass() : llvm::FunctionPass(ID), PMDataManager(), PMTopLevelManager(new llvm::FPPassManager()), F(nullptr), TTI(nullptr) {
		llvm::initializeLoopInfoWrapperPassPass(*llvm::PassRegistry::getPassRegistry());
		llvm::initializeScalarEvolutionWrapperPassPass(*llvm::PassRegistry::getPassRegistry());
		llvm::initializeCostModelAnalysisPass(*llvm::PassRegistry::getPassRegistry());
		llvm::initializeTargetTransformInfoWrapperPassPass(*llvm::PassRegistry::getPassRegistry());
	}

	unsigned getInstructionCost(const llvm::Instruction *I) const;
	
	bool runOnFunction(llvm::Function &F) override;
	void print(llvm::raw_ostream &OS, const llvm::Module*) const;

	llvm::PMDataManager *getAsPMDataManager() override { return this; }
	llvm::Pass *getAsPass() override { return this; }

	llvm::PassManagerType getTopLevelPassManagerType() override {
		return llvm::PMT_BasicBlockPassManager;
	}

	
private:

	void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
		
	/// The function that we analyze.
	llvm::Function *F;
	/// Target information.
	const llvm::TargetTransformInfo *TTI;
};


