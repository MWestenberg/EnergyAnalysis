#pragma once
class AnalysisVisitor
{	
public:
	virtual ~AnalysisVisitor() {};
	virtual void visit(class EnergyModule& em) = 0;

	// helper function to get the node label when it is just a number
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