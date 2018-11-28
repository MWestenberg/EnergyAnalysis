#pragma once

#include "EnergyAnalysis.h"
#include "../Constants.h"
#include "../Passes/WCETAnalysisPass.h"
#include <limits.h>



class WCETAnalysisVisitor : public AnalysisVisitor
{
private:
	
	// A heap allocated Cost map per function of type FunctionCostMap
	FunctionCostMap* FCM = new FunctionCostMap();

public:
	
	// Inherited via AnalysisVisitor
	virtual int visit(EnergyModule & em) override;

	FunctionCostMap& GetFunctionCostMap() const {
		return *FCM;
	}
	
	//Print out all costs to the console
	void Print();

	// Destructor deleting heap allocated FCM
	~WCETAnalysisVisitor() {
		delete(FCM);
	}

private:

	void SetFunctionCost(llvm::Function& F, llvm::BasicBlock& B, InstructionCostVec BCM);


};

