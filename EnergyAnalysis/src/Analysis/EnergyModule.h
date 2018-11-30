#pragma once
#include "llvm/IR/Module.h"
#include "AnalysisVisitor.h"


class EnergyModule
{
private:
	llvm::Module* m_module;

public:
	EnergyModule(llvm::Module& module) : m_module(&module) {};

	int accept(AnalysisVisitor& a);

	llvm::Module& GetLLVMModule()
	{
		return *m_module;
	}
	
	

};