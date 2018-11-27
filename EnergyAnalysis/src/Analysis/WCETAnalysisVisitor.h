#pragma once

#include "EnergyAnalysis.h"
#include "../Constants.h"
#include "../Passes/WCETAnalysisPass.h"

class WCETAnalysisVisitor : public AnalysisVisitor
{

public:

	
	// Inherited via AnalysisVisitor
	virtual void visit(EnergyModule & em) override;


	void CalculateCost(llvm::Function& F);
private:
	
	llvm::legacy::FunctionPassManager* pass;
	llvm::Module* module;
};

