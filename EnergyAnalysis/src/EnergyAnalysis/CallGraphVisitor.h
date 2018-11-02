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

#include "EnergyAnalysis.h"
#include "AnalysisVisitor.h"
#include <iostream>
#include <algorithm>


#include "llvm/ADT/PostOrderIterator.h"

class CallGraphVisitor : public AnalysisVisitor
{
private:
	
	typedef std::vector<llvm::BasicBlock*> OrderedBB;
	typedef std::map<llvm::Function*, OrderedBB> OrderedFunctions;
	typedef std::map<llvm::Function*, OrderedBB> SCCsinFunction;
	
	OrderedFunctions OrderedF;
	SCCsinFunction SCCs;
	
	

public:
	CallGraphVisitor();
	~CallGraphVisitor();
		

	// Inherited via AnalysisVisitor
	virtual void visit(EnergyModule& em) override;
private:
	llvm::Function* GetModuleEntryPoint(llvm::Module& M);

	void CallGraphVisitor::IterateSCC(llvm::BasicBlock& BB);
	void PostOrderTraversal(llvm::Module& M);
	void TraverseFunction(llvm::Function& F);
	void TraverseInstructions(llvm::BasicBlock& BB);

};

