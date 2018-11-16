#pragma once
#include "Logging.h"
#include "Constants.h"

class AnalysisVisitor
{	
public:
	AnalysisVisitor() { log.SetLevel(LOGLEVEL); };
	virtual ~AnalysisVisitor() {};
	virtual void visit(class EnergyModule& em) = 0;

protected:
	Logging log;
	

	//// LLVM global annotation function
	//const std::string LLVM_GLOBAL_ANNOTATIONS = "llvm.global.annotations"; 
	//// The name given to functions that control external hardware components and are energy annotated
	//const std::string ENERGY_ATTR = "Energy"; 
	//// External hardware functions are prefixed with _ and are removed after annotation
	//const std::string ENERGY_FUNCTION_PREFIX = "_"; 
	//// the name of the energy function as defined by the user
	//const std::string ENERGY_FUNCTION_NAME = "name";
	//// temporal power draw of energy in joules
	//const std::string ENERGY_TEMPORAL_CONSUMPTION = "pd"; 
	//// single power draw of energy in Joules
	//const std::string ENERGY_CONSUMPTION = "ec"; 
	//// time unit used in WCET
	//const std::string ENERGY_TIME_UNIT = "t"; 
	//// Function name of the main entry point
	//const std::string MODULE_ENTRY_POINT = "main"; 
	////function name to retrieve loop trip counts from undefined loops.
	//const llvm::StringRef LOOP_TRIPCOUNT = "LOOP_TRIPCOUNT"; 

public:
	// Useful helper function to print out a node name. 
	// In LLVM basic blocks are often defined as numbers like %3
	// This function tries to evaluate the node and returns a possible value as string
	std::string getSimpleNodeLabel(llvm::BasicBlock* Node)
	{

		if (!Node->getName().empty())
			return Node->getName().str();

		std::string Str;
		llvm::raw_string_ostream OS(Str);

		Node->printAsOperand(OS, false);
		return OS.str();
	}
};