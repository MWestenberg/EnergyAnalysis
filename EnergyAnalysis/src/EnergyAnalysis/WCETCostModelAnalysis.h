#pragma once
#include "Constants.h"
#include "EnergyAnalysis.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Analysis/TargetTransformInfoImpl.h"
#include "llvm/IR/LegacyPassManagers.h"


class WCETCostModelAnalysis : public llvm::FunctionPass, public llvm::PMDataManager, public llvm::PMTopLevelManager
{

public:
	static char ID; // Class identification, replacement for typeinfo
	WCETCostModelAnalysis() : llvm::FunctionPass(ID), PMDataManager(), PMTopLevelManager(new llvm::FPPassManager()), F(nullptr), TTI(nullptr) {
		llvm::initializeCostModelAnalysisPass(*llvm::PassRegistry::getPassRegistry());
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


