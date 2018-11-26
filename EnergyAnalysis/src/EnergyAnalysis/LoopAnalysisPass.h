#pragma once


#include "EnergyAnalysis.h"
#include "llvm/IR/Constants.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/LegacyPassManagers.h"
#include "llvm/IR/Dominators.h"
#include "AnalysisVisitor.h"

class LoopAnalysisPass : public llvm::FunctionPass, public llvm::PMDataManager, public llvm::PMTopLevelManager, public Analysis
{
private:

	// The function that we analyze.
	llvm::Function *F;

	// LLVM ScalarEvolution
	llvm::ScalarEvolution* SE;

	llvm::LoopInfo* LI;

	typedef std::vector<Edge> Edges;
	Edges loopEdges;

public:

	
	static char ID; // Class identification, replacement for typeinfo
	
	// Pass Constructor
	LoopAnalysisPass() : llvm::FunctionPass(ID), PMDataManager(), PMTopLevelManager(new llvm::FPPassManager()), F(nullptr), SE(nullptr) {
		llvm::initializeLoopInfoWrapperPassPass(*llvm::PassRegistry::getPassRegistry());
		llvm::initializeScalarEvolutionWrapperPassPass(*llvm::PassRegistry::getPassRegistry());
	}

	//Override of PMDataManager
	llvm::PMDataManager *getAsPMDataManager() override { return this; }

	// Override of Pass
	llvm::Pass *getAsPass() override { return this; }

	//Override of PMTOPLevelManager
	llvm::PassManagerType getTopLevelPassManagerType() override {
		return llvm::PMT_BasicBlockPassManager;
	}

	// Returns the SE
	llvm::ScalarEvolution &getSE() { return *SE; }
	// Const version of previos method
	const llvm::ScalarEvolution &getSE() const { return *SE; }

	//Override of FunctionPass
	bool runOnFunction(llvm::Function &F) override;
	
	Edges GetLoopEdges() const {
		return loopEdges;
	}

	
	~LoopAnalysisPass() {
		std::cout << "LoopAnalysisPass was destroyed" << std::endl;
	}

private:
	unsigned FindNestedLoops(llvm::Loop & L);

	// LLVM Anaysis usage method to preserve pass data
	void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;

	// Find loops that are undefined but maybe have loop tripcount annotation
	unsigned GetAnnotatedLoopTripCount(llvm::Loop& L);

	// Returns true of the function has the name defined as StringRef
	bool HasFunctionName(const llvm::Function& F, const llvm::StringRef& name) const;

	// Returns a pointer to a function when the method can cast the instruction to a function
	// excluded from this result are functions that return true to HasEneryAnnotation and are not defined
	// or a nullptr
	llvm::Function* IsFunction(const llvm::Instruction& I) const;

	//Returns true when energy annotation was found on a function
	bool HasEnergyAnnotation(const llvm::Function& F) const;
	// Sames as previous function but now the instruction will be casted first via
	// IsFunction
	bool HasEnergyAnnotation(const llvm::Instruction& I) const;
	
	// Tries to calculate the loop trip count
	// It will try to use the ScalarEvolutionWrapperPass first.
	// if this returns 0 GetAnnotatedLoopCount will be called
	// TODO: when count of a loop 0 is undefined and the application should return an error
	int GetLoopTripCount(llvm::Instruction& I) const;


};

