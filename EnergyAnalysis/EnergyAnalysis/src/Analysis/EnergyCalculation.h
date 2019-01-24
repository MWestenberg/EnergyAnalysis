#pragma once
#include "AnalysisVisitor.h"
#include "EnergyAnalysis.h"
#include "llvm/IR/SymbolTableListTraits.h"
#include "../ProgressBar.h"

class WCETAnalysisVisitor;



//This class is used to store the final outcome of the energy calculation
// It can be used to store the total consumption and has functions to reset and to add previous blocks
class TotalEnergyConsumption
{
public:
	std::string name = "";	
	EnergyConsumption energyConsumption;
	ExternalComponents externalComponents;
	ExternalComponents externalComponentsTrace;


	// Add a TotalEnergyConsumption Object 
	// This was added to optimize the code
	// We assume when a BB has a certain name, then that name is unique.
	// When the name equals the name of the previous BB we know it is a loop and nothing has changed
	// in terms of consumption. This methods copies all the calculated values to the total values.
	
	void Add(TotalEnergyConsumption TEC)
	{
		energyConsumption.Joules += TEC.energyConsumption.Joules;
		energyConsumption.executionTime += TEC.energyConsumption.executionTime;
		energyConsumption.numberOfCycles += TEC.energyConsumption.numberOfCycles;
		for (ExternalComponent EC : TEC.externalComponents)
			AddComponentConsumption(EC);
	
		for (ExternalComponent ECT : TEC.externalComponentsTrace)
			externalComponentsTrace.push_back(ECT);
	}

	//Reset the object, clears all values;
	void Reset()
	{
		energyConsumption.Reset();
		externalComponents.clear();
		externalComponentsTrace.clear();

	}

private:

	//This component can only be used in case he Add function is used
	// The same code is executed in EnergyCalculation::AddPowerdraw() with the exception that it uses the values
	// stored in the Externalcomponent.
	void AddComponentConsumption(ExternalComponent extComp)
	{
		auto found = std::find_if(externalComponents.begin(), externalComponents.end(), [&extComp](const ExternalComponent& e) { return e.name == extComp.name;  });
		if (found != externalComponents.end())
		{
			found->energyConsumption.Joules += extComp.energyConsumption.Joules;
			found->energyConsumption.executionTime += extComp.energyConsumption.executionTime;
		}
	}


	
};


// This class controls the total calculation and uses TotalEnergyConsumption to store the results
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

	TotalEnergyConsumption TEC;

	TotalEnergyConsumption TECLastBB;

	unsigned m_totalBBs = 0;
	unsigned m_currentBB = 0;

	ProgressBar pBar;

	// This will be true when the first powerdrawing energy function was found
	bool m_firstRun = true;

public:
	//Constructor
	explicit EnergyCalculation(TraceMap traceMap, WCETAnalysisVisitor& wcet, Log::Level level) : m_traceMap(traceMap), wcetAnalysis(&wcet) 
	{ log.SetLevel(level); TEC.name = "TotalConsumption"; };
	
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


	void AddCycles(unsigned int cycles);
	void AddJoules(long double joules);
	void AddPowerdraw(const InstructionCost & IC);
	void AddExternalComponent(ExternalComponent c);
	void AddExecTime(long double time);
};

