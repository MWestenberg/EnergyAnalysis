#pragma once
#include "llvm/Analysis/CallGraph.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/ADT/SCCIterator.h"
//#include "llvm/IR/InstrTypes.h"
//#include "llvm/IR/InstIterator.h"
//#include "llvm/IR/Instructions.h"
//#include "llvm/Support/Debug.h"
//#include "llvm/Support/raw_ostream.h"
//
//#include "llvm/Transforms/Utils/Evaluator.h"
//#include "llvm/Analysis/CFG.h"
//#include "llvm/IR/CFG.h"
//#include <utility>

#include "llvm/IR/InstIterator.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/ScalarEvolution.h"


#include "EnergyAnalysis.h"
#include "AnalysisVisitor.h"
#include <iostream>
#include <algorithm>


// This struct is used to define loops
// It includes a sorted vector of all basic blocks that are part of the loop
// It includes a tripCount: how often should a loop be iterated. This is taken from the LOOP_TRIPCOUNT function defined in AnalysisVisitor.h
// LoopSize is the size of the vector
// currentTrip is a vairable used when the loops are iterated. It keeps track of the current iteration
struct LoopOrderedSet
{
	typedef std::vector<llvm::BasicBlock*> Loop;
	Loop loop;
	unsigned int tripCount = 0;
	unsigned int loopSize = 0;
	unsigned int currentTrip = 1;
};

class CallGraphVisitor : public AnalysisVisitor
{
private:
	// An orederd vector of Basic Blocks
	typedef std::vector<llvm::BasicBlock*> OrderedBB;
	// A map of functions with a vector of Basic Blocks. This map is sorted in topological order
	typedef llvm::DenseMap<llvm::Function*, OrderedBB> OrderedFunctions;
	// A map of Loops (or Strongly Connectected Components).
	// They key is a pointer to the last Basic Block of the loop. 
	// The value is the struct LoopOrderedSet and contains all information regarding the loop.
	typedef llvm::DenseMap<llvm::BasicBlock*, LoopOrderedSet> SCCs;
	
	OrderedFunctions OrderedF; //Declaration of OrderedFunctions
	SCCs LoopSet; // Declaration of entire set of loops

	
public:
	// Inherited via AnalysisVisitor
	// Vistit method
	virtual void visit(EnergyModule& em) override;

	//Method to find the entry point
	//At this point a fixed value in AnalysisVisitor.h
	//Looks up the main entry point by name and returns a pointer to it.
	llvm::Function* GetModuleEntryPoint(llvm::Module& M);

private:

	// Basic blocks are not in topological order by default in LLVM. 
	// To calculate WCET and energy consumption these must be ordered first
	// and are added to OrderedF.
	void SortBasicBlocks(llvm::Module& M);

	// The argument passsed must be an ordered set of Basic Blocks that are a direct result
	// of the llvm::po_iterator<llvm::BasicBlock*>
	// This method is called by SortBasicBlocks()
	void CallGraphVisitor::LoopLookup(OrderedBB& SCCBBs);
		
	// This methods iterates each functions by requesting a Ordered Set of Basic blocks
	// The firstrun boolean is required because the first run is not useful until a
	// Energy annotated function is called. 
	// This methods traverses loops through each iteration of each Basic Blocks,
	// iterates nested functions and iterates loops (including nested loops) defined in LoopSet
	void IterateFunction(OrderedBB& OBB, bool firstRun);
	

/* ========================== HELPER METHODS ================================*/
public:
	//Check if the instruction passed is a function call
	llvm::Function* IsFunction(llvm::Instruction& I) const;


	//overloaded method that requires a function reference instead of an Instruction reference
	bool HasEnergyAnnotation(const llvm::Function& F) const;

private:
	//check if the instruction that was passed has an Energy Annotation
	//It does this by calling IsFunction first and then the overloaded method
	// HasEnergyAnnotation that requires a function reference
	bool HasEnergyAnnotation(llvm::Instruction& I) const;


	//Method that returns true if the given name is given to the function referenced
	bool HasFunctionName(const llvm::Function& F, const llvm::StringRef& name) const;

	//Checks the passed intruction reference and tries to get the first parameter
	//of the function call. This must be of type integer.
	//Also this function expects that the instruction is a function call to LOOP_TRIPCOUNT(unsigned int)
	int GetLoopTripCount(llvm::Instruction& I) const;

	// This methods requires a temporary set of a LoopOrderedSet and a reference where the loop should be stored (nestedLoop)
	// It traverses the basic blocks in the stack from beginning to end
	// and adds them to the nestedloop reference.
	// It also sets the size of the loop and retrieves the TripCount which is added to the LoopOrderedSet nestedloop reference
	void CreateLoopSet(LoopOrderedSet& stack, LoopOrderedSet& nestedLoop, llvm::BasicBlock* BBbegin, llvm::BasicBlock* BBend);
	
	//Traverse the LoopOrderedSet that is passed and looks for the LOOP_TRIPCOUNT function
	// It calls the GetLoopTripCount function and adds the tripCount to the passed reference.
	// When the LOOP_TRIPCOUNT function is found and the tripCount was added the function is also removed from the Module.
	void SetLoopTripCount(LoopOrderedSet& ls);

	// Returns the LoopSet that is mapped to the references Basic Block
	// The Basic Block must be the last of Basic Block of the loop in Topological order.
	LoopOrderedSet& GetLoop(llvm::BasicBlock& BB);

	// Returns true when the Basic Block is the last of Basic Block of a loop in topological order 
	// and was added to the LoopSet
	bool IsLoopStart(llvm::BasicBlock& BB);
	

	// BELOW SHOULD BE REMOVED WHEN READY
	void PostOrderTraversal(llvm::Module& M);
	void TraverseFunction(llvm::Function& F);
	void TraverseInstructions(llvm::BasicBlock& BB);
	void LoopCountTest(llvm::Function& M);
	void Print(OrderedBB& F);
	void PrintAllLoops();

	std::map<llvm::StringRef, llvm::StringRef> m_eFunctions; //temp store

};

