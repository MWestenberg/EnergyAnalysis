#pragma once
#include "Log.h"
#include "Constants.h"
#include <iostream>
#include <string>
#include <fstream>

//LLVM framework
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/support/raw_os_ostream.h"
#include <string>


class Analysis
{
public:
	// Useful helper function to print out a node name. 
	// In LLVM basic blocks are often defined as numbers like %3
	// This function tries to evaluate the node and returns a possible value as string
	std::string getSimpleNodeLabel(const llvm::BasicBlock* Node) const
	{

		if (!Node->getName().empty())
			return Node->getName().str();

		std::string Str;
		llvm::raw_string_ostream OS(Str);

		Node->printAsOperand(OS, false);
		return OS.str();
	}
protected:

	//// Returns true of the function has the name defined as StringRef
	bool HasFunctionName(const llvm::Function& F, const llvm::StringRef& name) const;

	// Returns a pointer to a function when the method can cast the instruction to a function
	// excluded from this result are functions that return true to HasEneryAnnotation and are not defined
	// or a nullptr
	llvm::Function* IsFunction(const llvm::Instruction& I) const;

	llvm::Function* IsFunctionCall(const llvm::Instruction& I) const;
	//Returns true when energy annotation was found on a function
	bool HasEnergyAnnotation(const llvm::Function& F) const;
	// Sames as previous function but now the instruction will be casted first via
	// IsFunction
	bool HasEnergyAnnotation(const llvm::Instruction& I) const;

	bool IsNotTraversable(llvm::Function& F) const;



protected:

	//Main Log function
	// To log to std out use Log.LogConsole()
	Log log;
};

class AnalysisVisitor : public Analysis
{
public:
	AnalysisVisitor() { log.SetLevel(LOGLEVEL); };
	virtual ~AnalysisVisitor() {};
	virtual int visit(class TraceModule& em) = 0;

	// A path consist of an ordered set of BasicBlocks
	typedef std::vector<llvm::BasicBlock*> OrderedBBSet;

	// Each Path will get a unique ID and is stored in a map
	typedef llvm::DenseMap<unsigned, OrderedBBSet> PathMap;

	typedef llvm::DenseMap<llvm::Function*, PathMap> FunctionPaths;


};