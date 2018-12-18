#pragma once
#include "EnergyAnalysis.h"
#include "AnalysisVisitor.h"
#include "llvm/Analysis/CFG.h"


class PathAnalysisVisitor: public AnalysisVisitor
{
public:
	// Visitor method override from AnalysisVisitor
	virtual int visit(EnergyModule & em) override;

	bool ProfilePath(llvm::Function& F);

	//Printer
	void Print();

private:
	friend class EnergyCalculator;

	// The current pathID
	unsigned int pathID = 0;

	//llvm::DenseMap<unsigned, OrderedBBSet>
	PathMap paths;

	//llvm::DenseMap<llvm::Function*, PathMap> 
	FunctionPaths fp;

	//Edges are a map of paths between 2 basic blocks
	// this is used to search and create possible paths which 
	// are later used to calculate the WCET of a function by calculating the cost of each path
	Edges loopEdges;

	// ===========  Member methods =============== //

	// Find and set the backedges of Loops
	// It adds a loop edge from exit to entre
	// e.g. %25 => %6 means loop exits at %25 and returns to %6
	void SetBackEdges(const llvm::Function& F);

	// Path analysis method. 
	// Defines all possible paths and stores these in PathMap
	// The map in turn is stored per function
	void CreatePath(llvm::BasicBlock& bb);


	//Returns FunctionPaths (Map: <llvm::Function*, PathMap>
	// Where PathMap is a Map on it's own consisting of a path identifier (unsigned)
	// and a OrderedBBSet which is an ordered vector of Basic Blocks;
	FunctionPaths& GetFunctionPaths() {
		return fp;
	}
	// Returns a PathMap of a function. These are all possible paths a function has
	// The return map is has as key an unsigned int and a OrderedBBSet as value;
	PathMap& GetFunctionPaths(llvm::Function& F) {
		return fp[&F];
	}

	//Printing of path summary. Made for debugging purposes
	void PrintPathSummary(unsigned pathID);

	// Helper method to test printing of a set used for debugging purposes only
	void printPath(const OrderedBBSet& path);


};

