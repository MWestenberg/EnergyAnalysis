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

};

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
	unsigned int loopTripCount = 0;

	//Copy constructor
	Edge(const Edge& other)
	{
		from = other.from;
		to = other.to;
		isLoop = other.isLoop;
		loopTripCount = other.loopTripCount;
	}

	// Compares two edges based on the edge
	// This ignores the fact we are dealing with a loop or not.
	// Returns true of both from and to basic blocks are equal
	bool operator==(const Edge& rhs)
	{
		return from == rhs.from && to == rhs.to;
	}

	// Can be used to compare an edge with a basic block
	// Returns true of the from basic block of the edge is equal to the passed basic block
	bool operator^=(const llvm::BasicBlock& rhs) {
		return from == &rhs;
	}

	//Returns true if it is marked as a loop
	bool IsLoop() const {
		return isLoop;
	}

	//Prints the edge to the console
	void Print()
	{
		llvm::StringRef l = (IsLoop() ? "Yes" : "No");
		log.LogConsole("Edge: " + getSimpleNodeLabel(from) + " => " + getSimpleNodeLabel(to) + "\n");
		log.LogConsole("  IsLoop: " + l.str());

	}
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

	//Copy Constructor
	InstructionCost(const InstructionCost& rhs) {
		SetIntruction(rhs.instruction);	SetInstructionCost(rhs.InstrCost); SetParentBasicBlock(rhs.parentBB);
	}

	//Set the Instruction pointer
	void SetIntruction(llvm::Instruction* inst) { instruction = inst; }

	//Set the Instruction Cost
	void SetInstructionCost(unsigned cost) { InstrCost = cost; }

	//Set the parent Basic block
	void SetParentBasicBlock(llvm::BasicBlock* BB) { parentBB = BB; }

	// Calling this method will add the cummulative costs
	// this will be the max cost of a basic block for the last instruction
	void AddCumulativeCost(unsigned cost) {	cummulativeCost = cost;}
};


// A vector containing InstuctionCost Objects
typedef std::vector<InstructionCost> InstructionCostVec;
// A dense map containing Pointers to Basic Blocks as key with an a vector of instructions as value
typedef llvm::DenseMap<llvm::BasicBlock*, InstructionCostVec> BBCostMap;
// A map dense map of function pointers with a BBCostMap as value (check BBCostMap for more info).
typedef llvm::DenseMap<llvm::Function*, BBCostMap> FunctionCostMap;