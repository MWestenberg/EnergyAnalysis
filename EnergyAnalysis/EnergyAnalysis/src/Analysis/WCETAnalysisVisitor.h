#pragma once

#include "EnergyAnalysis.h"
#include "AnalysisVisitor.h"
#include "../Passes/WCETAnalysisPass.h"
#include <limits.h>




//Instruction costs are expressed in number of cycles. How long 1 cycle requires depends on the 
//Speed of the controller. E.g. a 16 Mhz controller can do 16 million cycles per second.
//This means 1 cycle = to 0.0625 seconds. This is calculated in the WCETANalysisVisitor.
//The speed of the controller is expressed in Mhz, i.e. 16 for 16Mhz.
//In this class we divide the number of cycles by this value.
//E.g. 1 cycle equals to 1/16 = 0.0625 microseconds and not seconds.
//To retrieve the number of seconds the value must be divided by 16.000.000 in this example.
//this final division is done in the EnergCalculation class.
class WCETAnalysisVisitor : public AnalysisVisitor
{
private:
	friend class EnergyCalculation;
	// A heap allocated Cost map per function of type FunctionCostMap
	FunctionCostMap* FCM = new FunctionCostMap();

	long double m_clockspeed = DEFAULT_SPEED;

public:
	
	//Constructors
	WCETAnalysisVisitor() {};
	WCETAnalysisVisitor(int clockspeed) : m_clockspeed(clockspeed) {};
	WCETAnalysisVisitor(Log::Level level) { log.SetLevel(level); };
	WCETAnalysisVisitor(Log::Level level, int clockspeed) : m_clockspeed(clockspeed)  { log.SetLevel(level); };

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

