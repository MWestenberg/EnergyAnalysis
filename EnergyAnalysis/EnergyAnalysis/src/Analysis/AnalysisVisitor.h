#pragma once
#include "../Log.h"
#include "../Constants.h"
#include <iostream>
#include <string>
#include <fstream>

//LLVM framework
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/support/raw_os_ostream.h"
#include <string>

// This class hold a value for Energy consumption
// Consumption can be temperal, non temperal or incidental and has a time in microseconds.
struct EnergyValue
{
	llvm::StringRef name;
	unsigned pd = 0;
	unsigned ec = 0;
	long double t = 0.0;
	
	//Default constructor
	EnergyValue() {};

	//Overloaded Constructor
	EnergyValue(llvm::StringRef newName) : name(newName) {};

	//Copy Constructor
	EnergyValue(const EnergyValue& other)
	{
		name = other.name;
		pd = other.pd;
		ec = other.ec;
		t = other.t;
	}

	//Returns true if the name was set. 
	//When the name is set the constructor was called and the name is set to a value
	bool HasValues()
	{
		if (!name.empty())
			return true;
		return false;
	}

	// Compare this object of LHS with RHS
	// returns true when names are equal, false otherwise
	bool operator==(const EnergyValue& other)
	{
		return name == other.name;
	}
	
	// Compare this object of LHS with RHS
	// returns true when names are NOT equal, false otherwise
	bool operator!=(const EnergyValue& other)
	{
		return name != other.name;
	}

	// Returns true if this Energy Value has a temperal power consumption
	bool HasPowerDraw() { return pd > 0; }
};

// A vector of EnergyValue objects
typedef std::vector<EnergyValue> EnergyValueVec;

class Analysis
{
public:

	// Useful helper function to print out a node name. 
	// In LLVM basic blocks are often defined as numbers like %3
	// This function tries to evaluate the node and returns a possible value as string
	std::string getSimpleNodeLabel(const llvm::BasicBlock* Node) const
	{

		if (!Node->getName().empty())
			return Node->getName().str();

		std::string Str;
		llvm::raw_string_ostream OS(Str);

		Node->printAsOperand(OS, false);
		return OS.str();
	}

	void Tokenize(std::vector<llvm::StringRef>& tokens, const llvm::StringRef & text, char sep) const;
	
	
protected:

	//// Returns true of the function has the name defined as StringRef
	bool HasFunctionName(const llvm::Function& F, const llvm::StringRef& name) const;

	// Returns a pointer to a function when the method can cast the instruction to a function
	// excluded from this result are functions that return true to HasEneryAnnotation and are not defined
	// or a nullptr
	llvm::Function* IsFunction(const llvm::Instruction& I) const;

	//Returns true when energy annotation was found on a function
	bool HasEnergyAnnotation(const llvm::Function& F) const;
	// Sames as previous function but now the instruction will be casted first via
	// IsFunction
	bool HasEnergyAnnotation(const llvm::Instruction& I) const;

	llvm::Function* GetModuleEntryPoint(llvm::Module& M) const;

	bool IsNotTraversable(llvm::Function& F) const;

	EnergyValue GetEnergyValue(const llvm::Function&F) const;

	// Splits StringRef into separate tokens based on a separator.
	// All annotations are comma separated in a certain order.
	// Requires a reference to a vector


protected:

	//Main Log function
	// To log to std out use Log.LogConsole()
	Log log;
};

class AnalysisVisitor: public Analysis
{	
public:
	AnalysisVisitor() { log.SetLevel(LOGLEVEL); };
	virtual ~AnalysisVisitor() {};
	virtual int visit(class EnergyModule& em) = 0;

	// A path consist of an ordered set of BasicBlocks
	typedef std::vector<llvm::BasicBlock*> OrderedBBSet;

	// Each Path will get a unique ID and is stored in a map
	//typedef llvm::DenseMap<unsigned, OrderedBBSet> PathMap;
	//
	//typedef llvm::DenseMap<llvm::Function*, PathMap> FunctionPaths;
	

};


struct InstructionCost
{
	llvm::Instruction* instruction;
	llvm::BasicBlock* parentBB;

	unsigned InstrCycle = 0;
	long double InstrCost = 0.0;

	long double cummulativeCost = 0.0;
	//Default Constructor
	InstructionCost() : instruction(nullptr), parentBB(nullptr) {};

	//Overloaded Constructor
	InstructionCost(llvm::Instruction* inst) { SetIntruction(inst); }

	//Overloaded Constructor
	InstructionCost(llvm::Instruction* inst, llvm::BasicBlock* parent) { SetIntruction(inst); SetParentBasicBlock(parent); }

	//Overloaded Constructor
	InstructionCost(llvm::Instruction* inst, long double cost, unsigned cycles) { SetIntruction(inst); SetInstructionCost(cost); SetCycleCost(cycles); }

	//Overloaded Constructor
	InstructionCost(llvm::Instruction* inst, llvm::BasicBlock* parent, long double cost, unsigned cycles) {
		SetIntruction(inst); SetParentBasicBlock(parent); SetInstructionCost(cost); SetCycleCost(cycles);
	}

	//Overloaded Constructor
	InstructionCost(llvm::Instruction* inst, llvm::BasicBlock* parent, long double cost, unsigned cycles, long double cummulativeCost) {
		SetIntruction(inst); SetParentBasicBlock(parent); SetInstructionCost(cost); SetCycleCost(cycles);
		SetCumulativeCost(cummulativeCost);
	}

	//Copy Constructor
	InstructionCost(const InstructionCost& rhs) {
		SetIntruction(rhs.instruction);
		SetInstructionCost(rhs.InstrCost);
		SetCycleCost(rhs.InstrCycle);
		SetParentBasicBlock(rhs.parentBB);
		SetCumulativeCost(rhs.cummulativeCost);
	}

	//Set the Instruction pointer
	void SetIntruction(llvm::Instruction* inst) { instruction = inst; }

	//Set the Instruction Cost
	void SetInstructionCost(long double cost) { InstrCost = cost; }

	//Set the Instruction Cost
	void SetCycleCost(unsigned cycles) { InstrCycle = cycles; }

	//Set the parent Basic block
	void SetParentBasicBlock(llvm::BasicBlock* BB) { parentBB = BB; }

	// Calling this method will add the cummulative costs
	// this will be the max cost of a basic block for the last instruction
	void SetCumulativeCost(long double cost) { cummulativeCost = cost; }
};


// a map of traces
typedef llvm::DenseMap<llvm::StringRef, std::vector<llvm::StringRef>> TraceMap;

// A vector containing InstuctionCost Objects
typedef std::vector<InstructionCost> InstructionCostVec;

// A dense map containing Pointers to Basic Blocks as key with an a vector of instructions as value
typedef llvm::DenseMap<llvm::BasicBlock*, InstructionCostVec> BBCostMap;

// A map dense map of function pointers with a BBCostMap as value (check BBCostMap for more info).
typedef llvm::DenseMap<llvm::Function*, BBCostMap> FunctionCostMap;


// An Edge is an object that defines the relationship between 2 basic blocks.
// This relationship can also hold a boolean value in case it is a loop plus a tripcount 
struct Edge : public Analysis
{

	llvm::BasicBlock* from;
	llvm::BasicBlock* to;

	//Default Constructor
	Edge() {};

	//Overloaded Constructor with 2 basic block pointers
	Edge(llvm::BasicBlock* lhs, llvm::BasicBlock* rhs) : from(lhs), to(rhs) {};

	bool isLoop = false;
	bool isSubLoop = false;
	unsigned int loopTripCount = 0;
	unsigned int currentTripCount = 1;

	bool wasTraversed = false;

	unsigned int startJoules = 0;
	unsigned int startWcet = 0;

	unsigned int joules = 0;
	unsigned int wcet = 0;

	//Copy constructor
	Edge(const Edge& other)
	{
		from = other.from;
		to = other.to;
		isLoop = other.isLoop;
		loopTripCount = other.loopTripCount;
		isSubLoop = other.isSubLoop;
		currentTripCount = other.currentTripCount;
	}

	// Compares two edges based on the edge
	// This ignores the fact we are dealing with a loop or not.
	// Returns true of both from and to basic blocks are equal
	bool operator==(const Edge& rhs)
	{
		return from == rhs.from && to == rhs.to;
	}

	bool operator==(const Edge* rhs)
	{
		return from == rhs->from && to == rhs->to;
	}

	// Check if RHS Block is equal the Edge.from block
	// Edge << BB
	bool operator<<(const llvm::BasicBlock& rhs) {
		return from == &rhs;
	}

	// Check if RHS Block is equal the Edge.to block
	// Edge >> BB
	bool operator>>(const llvm::BasicBlock& rhs) {
		return to == &rhs;
	}

	//Returns true if it is marked as a loop
	bool IsLoop() const {	return isLoop;	}

	//Returns true if it is marked as a loop
	bool IsSubLoop() const { return isSubLoop;}

	//Prints the edge to the console
	void Print()
	{
		llvm::StringRef l = (IsLoop() ? "Yes" : "No");
		log.LogConsole("Edge: " + getSimpleNodeLabel(from) + " => " + getSimpleNodeLabel(to) + "\n");
		log.LogConsole("  IsLoop: " + l.str());

	}
};

// A vector of Edge object pointers
typedef std::vector<Edge*> Edges;

// A collection class that holds multiple edges that are part of a hierarchy
class EdgeCollection
{
private:
	Edges m_edges;
	
	
public:

	EdgeCollection() {};

	//Copy contructor
	EdgeCollection(const EdgeCollection& ec)
	{
		for (Edge* e : ec.GetAllLoopEdges())
		{
			this->AddEdge(*e);
		}
	}


	Edges GetAllLoopEdges() const {
		return m_edges;
	}

	void AddEdge(Edge e)
	{
		Edge* newEdge = new Edge(e);
		m_edges.push_back(newEdge);
		
	}

	bool IsLoopStart(llvm::BasicBlock* BB)
	{
		for (Edge* e : m_edges)
		{
			if (*e << *BB)
				return true;
		}
		return false;
	}

	bool IsLoopEnd(llvm::BasicBlock* BB)
	{
		for (Edge* e : m_edges)
		{
			if (*e >> *BB)
				return true;
		}
		return false;
	}


	Edge* GetLoopEdge(llvm::BasicBlock* BB)
	{
		Edge* loopEdge = nullptr;
		for (Edge* e : m_edges)
		{
			if (*e >> *BB)
				loopEdge = e;
		}

		return loopEdge;
	}

	~EdgeCollection()
	{

		//std::cout << "EdgeCollection  "<< ID << " will be destroyed" << std::endl;
		for (Edge* e : m_edges)
		{
			if (e != nullptr)
				delete e;
		}
	}

};

