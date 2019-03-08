#pragma once
#include "Analysis.h"

class TraceModule
{
private:
	llvm::Module* m_module;

public:
	explicit TraceModule(llvm::Module& module) : m_module(&module) {};

	int accept(AnalysisVisitor& a);
	llvm::Module& GetLLVMModule() { return *m_module; }

};
