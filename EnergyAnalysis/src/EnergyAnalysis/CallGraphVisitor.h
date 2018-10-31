#pragma once
#include "llvm/Analysis/CallGraph.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Transforms/Utils/Evaluator.h"

#include "EnergyAnalysis.h"
#include "AnalysisVisitor.h"
#include <iostream>

class CallGraphVisitor : public AnalysisVisitor
{
public:
	CallGraphVisitor();
	~CallGraphVisitor();
		

	// Inherited via AnalysisVisitor
	virtual void visit(EnergyModule& em) override;

};

