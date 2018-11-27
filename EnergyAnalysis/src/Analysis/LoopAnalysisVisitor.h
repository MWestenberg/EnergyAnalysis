#pragma once

#include "EnergyAnalysis.h"
#include "../Constants.h"
#include "../Passes/LoopAnalysisPass.h"

class LoopAnalysisVisitor: public AnalysisVisitor
{
private:
	typedef std::vector<Edge> Edges;
	Edges loopEdges;

public:
	// Visitor method override from AnalysisVisitor
	virtual void visit(EnergyModule & em) override;

	Edges GetLoopEdges() const {
		return loopEdges;
	}

	// Prints the loopedges found
	void Print();

};

