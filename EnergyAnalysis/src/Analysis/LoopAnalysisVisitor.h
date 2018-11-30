#pragma once

#include "EnergyAnalysis.h"
#include "AnalysisVisitor.h"
#include "../Passes/LoopAnalysisPass.h"

class LoopAnalysisVisitor: public AnalysisVisitor
{
private:
	friend class EnergyCalculator;

	Edges loopEdges;

public:
	// Visitor method override from AnalysisVisitor
	virtual int visit(EnergyModule & em) override;

	Edges GetLoopEdges() const {
		return loopEdges;
	}

	bool IsLoopStart(llvm::BasicBlock* BB)
	{
		for (Edge e : loopEdges)
		{
			if (e >> *BB)
				return true;
		}
		return false;
	}

	Edges GetLoopEdge(llvm::Function* F) {
		Edges loopsInF;

		for (auto& BB : *F)
		{
			for (Edge e : loopEdges)
			{
				if (e << BB)
					loopsInF.push_back(e);
			}
		}
		
		return loopsInF;
	}

	Edges GetLoopEdge(llvm::BasicBlock* BB) {
		Edges loopsInBB;
		
		for (Edge e : loopEdges)
		{
			if (e << *BB)
				loopsInBB.push_back(e);
		}
		return loopsInBB;
	}

	// Prints the loopedges found
	void Print();

};

