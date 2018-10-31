#pragma once
#include "llvm/IR/Module.h"
#include "AnalysisVisitor.h"

class EnergyModule
{
public:
	EnergyModule(llvm::Module& module) : m_module(&module) {};

	void accept(AnalysisVisitor& a);

	llvm::Module& GetLLVMModule()
	{
		return *m_module;
	}


private:
	llvm::Module* m_module;
		
};