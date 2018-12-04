#pragma once

#include "EnergyAnalysis.h"
#include "AnalysisVisitor.h"
#include "../Passes/LoopAnalysisPass.h"

class LoopAnalysisVisitor: public AnalysisVisitor
{
private:
	friend class EnergyCalculator;

	EdgeCollection loopEdges;

public:
	// Visitor method override from AnalysisVisitor
	virtual int visit(EnergyModule & em) override;

	EdgeCollection& GetLoopEdges() {
		return loopEdges;
	}
	

	//Returns true if the basic block is the latch /terminator block of a loop
	bool IsLoopStart(llvm::BasicBlock* BB)
	{
		return loopEdges.IsLoopStart(BB);
	}

	//Returns true if the basic block is the latch /terminator block of a loop
	bool IsLoopEnd(llvm::BasicBlock* BB)
	{
		return loopEdges.IsLoopEnd(BB);
	}

	Edge* GetLoopEdge(llvm::BasicBlock* BB)
	{
		return loopEdges.GetLoopEdge(BB);
	}

	//
	OrderedBBSet GetLoopOrderedBBSet(const Edge& loopEdge, const OrderedBBSet& path )
	{
		OrderedBBSet loopSet;
		bool loopBegan = false;
		for (llvm::BasicBlock* BB : path)
		{
			if (loopEdge.from == BB) {
				loopBegan = true;
				loopSet.push_back(BB);
			}
			else if (loopBegan)
				loopSet.push_back(BB);

			if (loopEdge.to == BB) 
				break;
		}

		return loopSet;
	}

	/*Edges GetLoopEdge(llvm::Function* F) {
		return loopEdges.GetLoopEdge(F);
	}

	Edges GetLoopEdge(llvm::BasicBlock* BB) {
		return loopEdges.GetLoopEdge(BB);
	}*/

	// Prints the loopedges found
	void Print();

};

