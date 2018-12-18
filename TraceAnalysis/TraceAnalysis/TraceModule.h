#pragma once
#include "Analysis.h"

class TraceModule
{
private:
	llvm::Module* m_module;

public:
	TraceModule(llvm::Module& module) : m_module(&module) {};

	int accept(AnalysisVisitor& a);
	llvm::Module& GetLLVMModule() { return *m_module; }

};
