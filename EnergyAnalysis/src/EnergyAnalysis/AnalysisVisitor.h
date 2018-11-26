#pragma once
#include "Logging.h"
#include "Constants.h"

class Analysis
{
public:
	std::string getSimpleNodeLabel(const llvm::BasicBlock* Node) const
	{

		if (!Node->getName().empty())
			return Node->getName().str();

		std::string Str;
		llvm::raw_string_ostream OS(Str);

		Node->printAsOperand(OS, false);
		return OS.str();
	}


	// Method to find the entry point
	// At this point a fixed value in AnalysisVisitor.h
	// Looks up the main entry point by name and returns a pointer to it.
	llvm::Function* GetModuleEntryPoint(llvm::Module& M) const
	{

		for (llvm::Function& F : M)
		{
			if (F.getName().str() == MODULE_ENTRY_POINT)
			{
				return &F;
			}
		}

		return nullptr;
	}
protected:
	Logging log;
};

class AnalysisVisitor: public Analysis
{	
public:
	AnalysisVisitor() { log.SetLevel(LOGLEVEL); };
	virtual ~AnalysisVisitor() {};
	virtual void visit(class EnergyModule& em) = 0;


	
public:
	// Useful helper function to print out a node name. 
	// In LLVM basic blocks are often defined as numbers like %3
	// This function tries to evaluate the node and returns a possible value as string
	
};


struct Edge : public Analysis
{
	llvm::BasicBlock* from;
	llvm::BasicBlock* to;
	Edge() {};
	Edge(llvm::BasicBlock* lhs, llvm::BasicBlock* rhs) : from(lhs), to(rhs) {};

	bool isLoop = false;
	unsigned int loopTripCount = 0;

	//copy constructor
	Edge(const Edge& other)
	{
		from = other.from;
		to = other.to;
		isLoop = other.isLoop;
		loopTripCount = other.loopTripCount;
	}

	bool operator==(const Edge& rhs)
	{
		return from == rhs.from && to == rhs.to;
	}

	bool operator==(const llvm::BasicBlock& fromBlock) {
		return from == &fromBlock;
	}

	void Print()
	{
		llvm::errs() << getSimpleNodeLabel(from) << " => " << getSimpleNodeLabel(to) << "\n";
	}

};