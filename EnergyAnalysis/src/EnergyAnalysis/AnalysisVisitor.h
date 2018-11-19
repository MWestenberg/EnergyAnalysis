#pragma once
#include "Logging.h"
#include "Constants.h"

class AnalysisVisitor
{	
public:
	AnalysisVisitor() { log.SetLevel(LOGLEVEL); };
	virtual ~AnalysisVisitor() {};
	virtual void visit(class EnergyModule& em) = 0;

protected:
	Logging log;
	
public:
	// Useful helper function to print out a node name. 
	// In LLVM basic blocks are often defined as numbers like %3
	// This function tries to evaluate the node and returns a possible value as string
	std::string getSimpleNodeLabel(llvm::BasicBlock* Node)
	{

		if (!Node->getName().empty())
			return Node->getName().str();

		std::string Str;
		llvm::raw_string_ostream OS(Str);

		Node->printAsOperand(OS, false);
		return OS.str();
	}
};