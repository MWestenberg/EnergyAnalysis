#pragma once
#include "EnergyAnalysis.h"
#include "Constants.h"
#include "llvm/Analysis/CFG.h"


class PathAnalysisVisitor: public AnalysisVisitor
{
public:
	// Visitor method override from AnalysisVisitor
	virtual void visit(EnergyModule & em) override;

	bool ProfilePath(llvm::Function& F);

	//Printer
	void PrintPaths();

private:
	// A path consist of an ordered set of BasicBlocks
	typedef std::vector<llvm::BasicBlock*> OrderedBBSet;

	// The current pathID
	unsigned int pathID = 0;

	// Each Path will get a unique ID and is stored in a map
	typedef llvm::DenseMap<unsigned, OrderedBBSet> PathMap;
	PathMap paths;

	typedef llvm::DenseMap<llvm::Function*, PathMap> FunctionPaths;
	FunctionPaths fp;

	// An edge between 2 basic blocks A -> B where A is the entry and B the successor
	typedef std::vector<Edge> Edges;

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

	bool HasEnergyAnnotation(const llvm::Function& F) const;
	bool HasFunctionName(const llvm::Function& F, const llvm::StringRef& name) const;

	// Helper method to test printing of a set used for debugging purposes only
	void printPath(const OrderedBBSet& path);


};

