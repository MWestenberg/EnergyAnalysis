#pragma once
#include "TraceAnalysis.h"
#include "Analysis.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/CallSite.h"
#include "llvm/Analysis/ValueTracking.h"
#include <stdarg.h>


class TransformVisitor : public AnalysisVisitor
{
	llvm::Module* module;
	llvm::Function* func_printf;

	unsigned NumReturnValProped;
	unsigned NumArgumentsProped;
public:
	TransformVisitor() : module(nullptr) {};
	TransformVisitor(Log::Level level) : module(nullptr) { log.SetLevel(level); };
	// Inherited via AnalysisVisitor
	virtual int visit(TraceModule & em) override;

	void WriteBitCodeFile(const llvm::StringRef& path);

	void Print();

private:
	void AddPrintFunction();


	std::vector<llvm::Instruction*> worklist;


	std::vector<llvm::Function*> unusedFunctions;
};

