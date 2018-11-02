#pragma once
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include <string>
#include <vector>
#include <iostream>


#include "EnergyAnalysis.h"
#include "AnalysisVisitor.h"

class TopoSorter : public AnalysisVisitor
{
public:
	// Inherited via AnalysisVisitor
	virtual void visit(EnergyModule & em) override;

	void runToposort(llvm::Function& F);
	void TopoSorter::PrintSorted();
private:
	enum Color {WHITE, GREY, BLACK};
	enum State { NEW, DETECTED, SORTED };

	typedef llvm::DenseMap<const llvm::BasicBlock*, Color> BBColorMap;
	typedef llvm::SmallVector<const llvm::BasicBlock*, 32> BBVector;

	BBColorMap ColorMap;
	BBVector SortedBBS;
	

	bool recursiveDFSToposort( llvm::BasicBlock* BB);
	void CheckForFunctions(llvm::BasicBlock& BB);
};