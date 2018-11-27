#pragma once

#include "EnergyAnalysis.h"
#include "../Constants.h"
#include "../Passes/WCETAnalysisPass.h"




class WCETAnalysisVisitor : public AnalysisVisitor
{
private:
	
	typedef std::vector<InstructionCost> InstructionCostVec;
	typedef llvm::DenseMap<llvm::BasicBlock*, InstructionCostVec> BBCostMap;
	typedef llvm::DenseMap<llvm::Function*, BBCostMap> FunctionCostMap;

	FunctionCostMap FCM;

public:
	
	// Inherited via AnalysisVisitor
	virtual void visit(EnergyModule & em) override;

	//Print out all costs
	void Print();
private:



	
};

