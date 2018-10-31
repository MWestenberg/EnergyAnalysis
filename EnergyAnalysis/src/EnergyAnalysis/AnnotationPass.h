#pragma once
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Type.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Support/raw_ostream.h"

#include "EnergyAnalysis.h"


class AnnotationPass : public llvm::ModulePass
{
public:
	static char ID;
	explicit AnnotationPass() : llvm::ModulePass(ID) {}
	bool runOnModule(llvm::Module& M);

	void PrintAnnotations(llvm::Module&) const;

private:
	//static std::vector<llvm::StringRef> Tokenize(const llvm::StringRef& text, char sep);
	
	void Tokenize(std::vector<llvm::StringRef>& tokens, const llvm::StringRef& text, char sep);

	void AddAnnotation(const llvm::Module &M);
	void SetEnergyFunctions(llvm::Module&);
	llvm::Function* GetEnergyFunction(const llvm::Function& fn);
	//void AnnotationPass::SetEnergyAttr(llvm::Function& fn) const;
	
	std::map<llvm::StringRef, llvm::Function*> m_EnergyFunctions;
	bool m_annotationedAdded = false;
};

