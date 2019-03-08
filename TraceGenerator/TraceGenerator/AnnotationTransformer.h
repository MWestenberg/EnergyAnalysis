#pragma once
#include "Analysis.h"
#include "TraceGenerator.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

class AnnotationTransformer :public AnalysisVisitor
{
private:
	//map of all functions excluding dummy function that start with ENERGY_FUNCTION_PREFIX (defined in AnalysisVistor.h). 
	std::map<llvm::StringRef, llvm::Function*> m_filteredFunctions;
	//helper boolean for assiting the Print method.
	bool m_annotationedAdded = false;

	int m_result = 0;

	llvm::Module* module;

public:
	AnnotationTransformer() : module(nullptr) {};
	AnnotationTransformer(Log::Level level) : module(nullptr) { log.SetLevel(level); };

	// Inherited via AnalysisVisitor
	virtual int visit(TraceModule & em) override;

	//function to print all annotations for the entire module. 
	//This is purely for debugging purposes
	// Annotation must be added. This is checked by the m_annotationedAdded boolean
	void Print();

private:

	//this function locates all annotations and tries to find the declared function that belongs to it
	// If found we know the declared function is an energy function.
	// Because we call the declared function to fool the compiler not to change the code we must transform
	// the call instruction with the defined version to allow for partial execution.
	bool locateAnnotations();

	//This function is called from locateAnnotations and requires an energy function (declared and called in in code)
	// defined as realEnergyFn. This call instruction must be replaced with the defined function to allow
	// execution. E.g. turnOn() is declared but not defined and called in code, _turnOn() is defined but not called in code
	// replace all calls to turnOn() to _turnOn();
	bool transformEnergyFunctionCalls(llvm::Function* realEnergyFn, llvm::Function* definedFunction);

	// Splits StringRef into separate tokens based on a separator.
	// All annotations are comma separated in a certain order.
	// Requires a reference to a vector
	void Tokenize(std::vector<llvm::StringRef>& tokens, const llvm::StringRef& text, char sep) const;

	// The EnergyAnalysis header implements dummy functions with  ENERGY_FUNCTION_PREFIX (defined in AnalysisVistor.h). 
	// this functions filters out those functions and assign the "real" functions to the map m_EnergyFunctions
	void SetEnergyFunctions();

	// Retrieves the orignal Energy Annotated function based on a declared function
	llvm::Function* GetEnergyFunction(const llvm::Function& fn);
};

