#pragma once
class AnalysisVisitor
{	
public:
	virtual ~AnalysisVisitor() {};
	virtual void visit(class EnergyModule& em) = 0;
};