#pragma once
#include "AnalysisVisitor.h"
#include "EnergyAnalysis.h"
#include <algorithm>

// The following classes are required for this unit
class PathAnalysisVisitor;
class LoopAnalysisVisitor;
class WCETAnalysisVisitor;


// In this class the paths created in PathAnalysisVisitor are used.
// It starts with the individual possible paths per function.
// In order to calulcate the cost the traversal must start at the main entry point.
// The cost of each instruction can be ignored until the first Energy Annotated function is 
// is located.
//
// In order to keep track of each path we must keep the subpath stored to
// calculate the cost of each possible path through the entire program (ProgramPath)
// The ProgramPaths are stored in member variable called ProgramPathCost
// 
// The path traversal is in topological order and keeps tracks of loops
// Example:
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
// Each programpath must be traversed because we do not now when an energy function is occuring
// In the class PathAnalysisVisitor the paths are created per function and are 
// labeled from 0 to a higher number
// In this class we traverse them as ProgramPaths if the function path = 0 we continue 
// When the function path id is > 0 we must add +1 to the ProgramPath ID
// As soon as we add +1 to the ProgramPath we must also copy the Path of the previous level
// This is where NestedLevel comes in. 
// The WCET and the total number of joules must be kept per NestedLevel.
// To make this easy a type is created for each of them: WCETPerLevel and JoulesPerlevel
// These are Maps holding an unsigned integer as value and NestedLevel (also unsigned) as key.
//
// In the example when we jump from node B to X the first path might be X-Z-W. This is Path 0
// in functionX. When this traversal is finished it moves on to X-Y-W. This is Path 1 in functionX.
// When this jump happens a new ProgramPath is created. The previous NestedLevel (WCET, Joules) must be copied 
// to keep track of the entire cost of the current ProgramPath
//



// this keeps track of the level of the program
// it is just an integer but it is important to keep track when jumping functions
// in case the function has multiple paths the nested level is required
typedef unsigned int NestedLevel;
typedef unsigned int WCET;
typedef unsigned int Joules;

// WCET per level to enable copying when jumping path a function
typedef llvm::DenseMap<NestedLevel, WCET> WCETperLevel;
//Joules per level to enable copying when jumping path in a function
typedef llvm::DenseMap<NestedLevel, Joules> JoulesPerlevel;
// Energy
typedef llvm::DenseMap<NestedLevel, EnergyValueVec> EnergyConsumptionPerLevel;



struct ProgramPath
{
	unsigned ID = 1;

	WCETperLevel wcet;
	JoulesPerlevel joules;
	EnergyConsumptionPerLevel energyConsumption;
	
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


	// Prints the WCET and Joules
	void Print();

private:
	llvm::Module* module;
	
	// This will be true when the first powerdrawing energy function was found
	bool m_firstRun = true;

	NestedLevel m_nestedLevel = 0;

	// The programpath ID
	// This is the final identifier for each single path through the entire program
	// starting from the main entry point
	typedef unsigned int ProgramPathID;
	ProgramPathID programPathID = 0;

	typedef std::map<ProgramPathID, ProgramPath> PathCosts;
	PathCosts ProgramPathCosts;

	unsigned int CurrentSubPath = 0;
	
private:

	void TraveseFunction(PathMap& pathMap);
	void TraversePaths(OrderedBBSet& OBB);

	void SetInstructionCost(llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I);
	bool SetEnergyFunctionCost(const llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I);
	void AddPath();

	void AddWCET(WCET executionTime);
	
	void AddJoules(Joules joules);
	void AddPowerdraw(const InstructionCost& IC);

	
};

