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
#include "AnalysisVisitor.h"

class AnnotationVisitor : public AnalysisVisitor
{
private:
	//map of all functions excluding dummy function that start with ENERGY_FUNCTION_PREFIX (defined in AnalysisVistor.h). 
	std::map<llvm::StringRef, llvm::Function*> m_filteredFunctions;
	//helper boolean for assiting the Print method.
	bool m_annotationedAdded = false; 
public:

	// Inherited via AnalysisVisitor
	virtual void visit(EnergyModule & em) override;
	
	//function to print all annotations for the entire module. 
	//This is purely for debugging purposes
	// Annotation must be added. This is checked by the m_annotationedAdded boolean
	void PrintAnnotations(llvm::Module& M);

private:
	// Splits StringRef into separate tokens based on a separator.
	// All annotations are comma separated in a certain order.
	// Requires a reference to a vector
	void Tokenize(std::vector<llvm::StringRef>& tokens, const llvm::StringRef& text, char sep);

	// Energy annotated functions start with an ENERGY_FUNCTION_PREFIX (defined in AnalysisVistor.h). 
	// These functions are declared and defined in the code.
	// The EnergyAnalysis header implements these via Macros and makes undefined functions without the ENERGY_FUNCTION_PREFIX.
	// This methods retrieves all global annotations and applies them to the correct functions (without prefix)
	// such that these functions have no definition just a declaration.
	// The definined dummy functions with prefix are removed
	void AddAnnotation(llvm::Module &M);

	// The EnergyAnalysis header implements dummy functions with  ENERGY_FUNCTION_PREFIX (defined in AnalysisVistor.h). 
	// this functions filters out those functions and assign the "real" functions to the map m_EnergyFunctions
	void SetEnergyFunctions(llvm::Module&);

	//Helper function that removes functions that are passed by reference.
	//This actually removes the functions from the Module
	void RemoveUnusedFunctions(llvm::Function&);

	// Retrieves the orignal Energy Annotated function based on a declared function
	llvm::Function* GetEnergyFunction(const llvm::Function& fn);
};