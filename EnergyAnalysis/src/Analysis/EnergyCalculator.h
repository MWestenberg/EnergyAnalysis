#pragma once
#include "AnalysisVisitor.h"
#include "EnergyAnalysis.h"
#include <algorithm>
class PathAnalysisVisitor;
class LoopAnalysisVisitor;
class WCETAnalysisVisitor;


// unsigned int is the subpath ID, the instructionCostVec is a vector including all cost of a subpath
typedef llvm::DenseMap<unsigned, InstructionCostVec> SubPaths;

typedef llvm::DenseMap<llvm::Function*, unsigned> CostFunction;

struct ProgramPath
{
	unsigned ID = 1;
	
	SubPaths subPaths;
	CostFunction wcetPerFn;
	CostFunction joulesPerFn;

	EnergyFunctions energyFunctions;

	unsigned int currentSubPath = 0;

	unsigned totalWcet = 0;
	unsigned totalJoules = 0;



};

class EnergyCalculator : public AnalysisVisitor
{
	PathAnalysisVisitor* pathAnalysis;
	LoopAnalysisVisitor* loopAnalysis;
	WCETAnalysisVisitor* wcetAnalysis;


public:
	
	EnergyCalculator(PathAnalysisVisitor& pa, LoopAnalysisVisitor& la, WCETAnalysisVisitor& wcet) 
		: pathAnalysis(&pa), loopAnalysis(&la), wcetAnalysis(&wcet) {};

	// Inherited via AnalysisVisitor
	virtual int visit(EnergyModule & em) override;
private:
	llvm::Module* module;
	
	bool m_firstRun = true;

	unsigned m_nestedLevel = 0;

	// The programpath ID
	// This is the final identifier for each single path through the entire program
	// starting from the main entry point
	typedef unsigned int ProgramPathID;
	ProgramPathID programPathID = 0;

	// In order to keep track of each path we must keep the subpath
	// The path traversal is kind of a DFS traversal including loops
	//         Main				functionX
	//         =====            ========= 
	//			A                  X
	//        /  \                /  \
	//       B    C              Z    Y
	//        \  /                \  /
	//          D                  W
	//
	// Created by PathAnalysisVisitor:
	// Main has 2 paths: (0) A-B-D   (1) A-C-D
	// functionX has 2 paths: (0) X-Z-W  (1) Z-Y-W
	// function X is called in block B
	// 
	// This leads to the following program paths
	// 1) A-B-X-Z-W-D
	// 2) A-B-X-Y-W-D
	// 3) A-C-D
	// 
	// Each must be traversed because we do not now when an energy function is occuring
	// In the class PathAnalysisVisitor the paths are created per function and are 
	// labeled from 0 to a higher number
	// In this class we traverse them as ProgramPaths if the function path = 0 we continue 
	// When the function path id is > 0 we must add +1 to the programpath ID
	// As soon as we add +1 to the programpath we must also the initial instruction costs 
	// of the previous function. This is where subpath comes in
	//
	// In the example we create a map like this:
	//
	//  1) subpath 0: A - B, subpath 1: X-Z-W, subpath 0, D
	//    By keeping track of the nested level we can traverse the paths.
	// 

	typedef std::map<ProgramPathID, ProgramPath> PathCosts;
	PathCosts ProgramPathCosts;

	unsigned int CurrentSubPath = 0;
	
private:

	void TraveseFunction(PathMap& pathMap);
	void TraversePaths(OrderedBBSet& OBB);

	void SetInstructionCost(llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I);
	bool SetEnergyFunctionCost(const llvm::Function& F);
	void AddPath();


	void testPrint();
};

