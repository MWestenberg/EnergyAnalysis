#pragma once
#include "AnalysisVisitor.h"
#include "EnergyAnalysis.h"
#include "llvm/IR/SymbolTableListTraits.h"

class WCETAnalysisVisitor;


class EnergyCalculation :public AnalysisVisitor
{

private:
	//The tracemap that was passed as file and converted to a vector
	TraceMap m_traceMap;
	
	//The execution cost of each instruction is decided by this class
	WCETAnalysisVisitor* wcetAnalysis;

	//the llvm module we are working on
	llvm::Module* module;

	// a vector of StringRef that holds the ID of the Basic Block.
	// for example, %3 is "3" in the set;
	typedef std::vector<llvm::StringRef> BBTraceSet;

	// A map of functions as key and a vector (OrderedBBSet) of basic blocks
	typedef llvm::DenseMap<llvm::Function*, OrderedBBSet>  PathMap;
	PathMap m_pathMap;

public:
	//Constructor
	explicit EnergyCalculation(TraceMap traceMap, WCETAnalysisVisitor& wcet) : m_traceMap(traceMap), wcetAnalysis(&wcet){};
	
	// Inherited via AnalysisVisitor
	virtual int visit(EnergyModule & em) override;

	// Prints the read in trace that is required for the analysis
	void PrintTrace();

	void Print();

private:

	//Function creates a trace with pointers to functions and basic blocks
	// and stores this in PathMap;
	bool CreateBBTrace();
	bool TraverseTrace(OrderedBBSet& OBB);
	void EnergyCalculation::SetInstructionCost(llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I);
	bool  SetEnergyFunctionCost(const llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I);
};

