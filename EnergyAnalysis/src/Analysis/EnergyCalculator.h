#pragma once
#include "AnalysisVisitor.h"
#include "EnergyAnalysis.h"

class PathAnalysisVisitor;
class LoopAnalysisVisitor;
class WCETAnalysisVisitor;

class EnergyCalculator : public AnalysisVisitor
{
	PathAnalysisVisitor* pathAnalysis;
	LoopAnalysisVisitor* loopAnalysis;
	WCETAnalysisVisitor* wcetAnalysis;
public:
	
	EnergyCalculator(PathAnalysisVisitor& pa, LoopAnalysisVisitor& la, WCETAnalysisVisitor& wcet) 
		: pathAnalysis(&pa), loopAnalysis(&la), wcetAnalysis(&wcet) {};

	// Inherited via AnalysisVisitor
	virtual int visit(EnergyModule & em) override;
private:
	llvm::Module* module;
	
	bool m_firstRun = true;

private:

	void TraveseFunction(PathMap& pathMap);
	void TraversePaths(OrderedBBSet& OBB);

	void SetInstructionCost(llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I);
	bool SetEnergyFunctionCost(llvm::Function& F);
};

