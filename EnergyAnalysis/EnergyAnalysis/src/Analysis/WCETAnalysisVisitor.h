#pragma once

#include "EnergyAnalysis.h"
#include "AnalysisVisitor.h"
#include "../Passes/WCETAnalysisPass.h"
#include <limits.h>



class WCETAnalysisVisitor : public AnalysisVisitor
{
private:
	friend class EnergyCalculation;
	// A heap allocated Cost map per function of type FunctionCostMap
	FunctionCostMap* FCM = new FunctionCostMap();

	long double m_clockspeed;

public:
	
	WCETAnalysisVisitor(int clockspeed = 16) : m_clockspeed(clockspeed) {};
	WCETAnalysisVisitor(Log::Level level, int clockspeed = 16) : m_clockspeed(clockspeed)  { log.SetLevel(level); };
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

	InstructionCost GetInstructionCost(llvm::Function& F, llvm::BasicBlock& BB,  llvm::Instruction& I);

};

