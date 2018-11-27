#pragma once

#include "EnergyAnalysis.h"
#include "Constants.h"
#include "WCETAnalysisPass.h"

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

