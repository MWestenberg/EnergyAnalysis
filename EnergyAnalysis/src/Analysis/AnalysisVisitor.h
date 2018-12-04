#pragma once
#include "../Logging.h"
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

protected:

	//Main Log function
	// To log to std out use Log.LogConsole()
	Logging log;
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
	typedef llvm::DenseMap<unsigned, OrderedBBSet> PathMap;
	
	typedef llvm::DenseMap<llvm::Function*, PathMap> FunctionPaths;
	

};


struct InstructionCost
{
	llvm::Instruction* instruction;
	llvm::BasicBlock* parentBB;

	unsigned InstrCost = 0;

	unsigned cummulativeCost = 0;
	//Default Constructor
	InstructionCost() : instruction(nullptr), parentBB(nullptr) {};

	//Overloaded Constructor
	InstructionCost(llvm::Instruction* inst) { SetIntruction(inst); }

	//Overloaded Constructor
	InstructionCost(llvm::Instruction* inst, llvm::BasicBlock* parent) { SetIntruction(inst); SetParentBasicBlock(parent); }

	//Overloaded Constructor
	InstructionCost(llvm::Instruction* inst, unsigned cost) { SetIntruction(inst); SetInstructionCost(cost); }

	//Overloaded Constructor
	InstructionCost(llvm::Instruction* inst, llvm::BasicBlock* parent, unsigned cost) {
		SetIntruction(inst); SetParentBasicBlock(parent); SetInstructionCost(cost);
	}

	//Overloaded Constructor
	InstructionCost(llvm::Instruction* inst, llvm::BasicBlock* parent, unsigned cost, unsigned cummulativeCost) {
		SetIntruction(inst); SetParentBasicBlock(parent); SetInstructionCost(cost);
		SetCumulativeCost(cummulativeCost);
	}

	//Copy Constructor
	InstructionCost(const InstructionCost& rhs) {
		SetIntruction(rhs.instruction);
		SetInstructionCost(rhs.InstrCost);
		SetParentBasicBlock(rhs.parentBB);
		SetCumulativeCost(rhs.cummulativeCost);
	}

	//Set the Instruction pointer
	void SetIntruction(llvm::Instruction* inst) { instruction = inst; }

	//Set the Instruction Cost
	void SetInstructionCost(unsigned cost) { InstrCost = cost; }

	//Set the parent Basic block
	void SetParentBasicBlock(llvm::BasicBlock* BB) { parentBB = BB; }

	// Calling this method will add the cummulative costs
	// this will be the max cost of a basic block for the last instruction
	void SetCumulativeCost(unsigned cost) { cummulativeCost = cost; }
};


// A vector containing InstuctionCost Objects
typedef std::vector<InstructionCost> InstructionCostVec;
// A dense map containing Pointers to Basic Blocks as key with an a vector of instructions as value
typedef llvm::DenseMap<llvm::BasicBlock*, InstructionCostVec> BBCostMap;
// A map dense map of function pointers with a BBCostMap as value (check BBCostMap for more info).
typedef llvm::DenseMap<llvm::Function*, BBCostMap> FunctionCostMap;
// An edge between 2 basic blocks A -> B where A is the entry and B the successor
//typedef std::vector<Edge> Edges;


typedef std::vector<const llvm::Function*> EnergyFunctions;





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

	unsigned int totalJoules = 0;
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

typedef std::vector<Edge*> Edges;
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

	/*Edges GetLoopEdges(llvm::BasicBlock* BB) {
		Edges loopsInBB;

		for (Edge* e : m_edges)
		{
			if (*e << *BB)
				loopsInBB.push_back(e);
		}
		return loopsInBB;
	}

	Edges GetLoopEdges(llvm::Function* F) {
		Edges loopsInF;

		for (auto& BB : *F)
		{
			for (Edge* e : m_edges)
			{
				if (*e << BB)
					loopsInF.push_back(e);
			}
		}

		return loopsInF;
	}*/


	
};

