#pragma once
#include "EnergyAnalysis.h"
#include "AnalysisVisitor.h"
#include "llvm/IR/Constants.h"



class AnnotationVisitor : public AnalysisVisitor
{
private:
	//map of all functions excluding dummy function that start with ENERGY_FUNCTION_PREFIX (defined in AnalysisVistor.h). 
	std::map<llvm::StringRef, llvm::Function*> m_filteredFunctions;
	//helper boolean for assiting the Print method.
	bool m_annotationedAdded = false; 

	int m_result = 0;
public:

	// Inherited via AnalysisVisitor
	virtual int visit(EnergyModule & em) override;
	
	//function to print all annotations for the entire module. 
	//This is purely for debugging purposes
	// Annotation must be added. This is checked by the m_annotationedAdded boolean
	void Print(llvm::Module& M);

private:
	// Splits StringRef into separate tokens based on a separator.
	// All annotations are comma separated in a certain order.
	// Requires a reference to a vector
	void Tokenize(std::vector<llvm::StringRef>& tokens, const llvm::StringRef& text, char sep) const;

	// Energy annotated functions start with an ENERGY_FUNCTION_PREFIX (defined in AnalysisVistor.h). 
	// These functions are declared and defined in the code.
	// The EnergyAnalysis header implements these via Macros and makes undefined functions without the ENERGY_FUNCTION_PREFIX.
	// This methods retrieves all global annotations and applies them to the correct functions (without prefix)
	// such that these functions have no definition just a declaration.
	// The definined dummy functions with prefix are removed
	bool AddAnnotation(llvm::Module &M);


	//Sets the attributes to the function as defined in the tokens
	// returns true of succesfull, false in case the function is a nullptr
	bool AddEnergyAttribute(llvm::Function& F, const std::vector<llvm::StringRef>& tokens);
	// The EnergyAnalysis header implements dummy functions with  ENERGY_FUNCTION_PREFIX (defined in AnalysisVistor.h). 
	// this functions filters out those functions and assign the "real" functions to the map m_EnergyFunctions
	void SetEnergyFunctions(llvm::Module&);

	//Helper function that removes functions that are passed by reference.
	//This actually removes the functions from the Module
	void RemoveUnusedFunctions(llvm::Function&) const;

	// Retrieves the orignal Energy Annotated function based on a declared function
	llvm::Function* GetEnergyFunction(const llvm::Function& fn);
};