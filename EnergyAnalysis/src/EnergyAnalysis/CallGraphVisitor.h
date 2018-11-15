#pragma once
#include "llvm/Analysis/CallGraph.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/ADT/SCCIterator.h"
//#include "llvm/IR/InstrTypes.h"
//#include "llvm/IR/InstIterator.h"
//#include "llvm/IR/Instructions.h"
//#include "llvm/Support/Debug.h"
//#include "llvm/Support/raw_ostream.h"
//
//#include "llvm/Transforms/Utils/Evaluator.h"
//#include "llvm/Analysis/CFG.h"
//#include "llvm/IR/CFG.h"
//#include <utility>

#include "llvm/IR/InstIterator.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/ScalarEvolution.h"


#include "EnergyAnalysis.h"
#include "AnalysisVisitor.h"
#include <iostream>
#include <algorithm>


struct LoopOrderedSet
{
	typedef std::vector<llvm::BasicBlock*> Loop;
	Loop loop;
	unsigned int tripCount = 0;
	unsigned int loopSize = 0;
	unsigned int currentTrip = 0;
};

class CallGraphVisitor : public AnalysisVisitor
{
private:
	
	const llvm::StringRef LOOP_TRIPCOUNT = "LOOP_TRIPCOUNT";

	typedef std::vector<llvm::BasicBlock*> OrderedBB;
	typedef llvm::DenseMap<llvm::Function*, OrderedBB> OrderedFunctions;
	typedef llvm::DenseMap<llvm::BasicBlock*, LoopOrderedSet> SCCs;
	
	OrderedFunctions OrderedF;
	//SCCsinFunction SCCs;
	SCCs LoopSet;
	
	std::map<llvm::StringRef, llvm::StringRef> m_eFunctions; //temp store

public:
	// Inherited via AnalysisVisitor
	virtual void visit(EnergyModule& em) override;
private:
	//Method to find the entry point
	llvm::Function* GetModuleEntryPoint(llvm::Module& M);

	void SortBasicBlocks(llvm::Module& M);
	void IterateFunction(OrderedBB& OBB, bool firstRun);
	

	//Helper methods
	llvm::Function* IsFunction(llvm::Instruction& I) const;
	bool HasEnergyAnnotation(llvm::Instruction& I) const;
	bool HasEnergyAnnotation(const llvm::Function& F) const;
	bool HasFunctionName(const llvm::Function& F, const llvm::StringRef& name) const;
	int GetLoopTripCount(llvm::Instruction& I) const;
	void CreateLoopSet(LoopOrderedSet& stack, LoopOrderedSet& nestedLoop, llvm::BasicBlock* BBbegin, llvm::BasicBlock* BBend);
	LoopOrderedSet& GetLoop(llvm::BasicBlock& BB);
	bool IsLoopStart(llvm::BasicBlock& BB);
	void SetLoopTripCount(LoopOrderedSet& ls);

	// BELOW ARE TEST FUNCTIONS
	void PostOrderTraversal(llvm::Module& M);
	void TraverseFunction(llvm::Function& F);
	void TraverseInstructions(llvm::BasicBlock& BB);
	void LoopCountTest(llvm::Function& M);
	void Print(OrderedBB& F);
	void PrintAllLoops();
};

