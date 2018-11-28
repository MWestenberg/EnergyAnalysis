#include "AnalysisVisitor.h"

bool Analysis::HasFunctionName(const llvm::Function& F, const llvm::StringRef& name) const
{
	if (F.getName().startswith_lower(name))
		return true;

	return false;
}


//Check if the instruction is a function. return nullptr in case it is not a function else a function pointer
llvm::Function* Analysis::IsFunction(const llvm::Instruction& I) const
{

	llvm::CallInst * callInst = llvm::dyn_cast<llvm::CallInst>(const_cast<llvm::Instruction*>(&I));
	if (callInst == nullptr)
		return nullptr;

	//check if it can be casted to a function pointer
	if (llvm::Function *tailCall = llvm::dyn_cast<llvm::Function>(callInst->getCalledValue()->stripPointerCasts()))
	{
		// Energy Functions and LOOP_TRIP_COUNT are exceptions that are not declared
		if (HasEnergyAnnotation(*tailCall) || HasFunctionName(*tailCall, LOOP_TRIPCOUNT))
			return tailCall;
		else if (!tailCall->doesNotAccessMemory() && !tailCall->isDeclaration()) // all undeclared methods can be ignored (= external)
			return tailCall;

	}

	return nullptr;
}


// Returns true when function has an Energy Annotation, is not delcared, 
// does not access any memory or has the name defined in LOOP_TRIPCOUNT
// otherwise false
bool Analysis::IsNotTraversable(llvm::Function& F) const
{
	if (&F == nullptr
		|| HasEnergyAnnotation(F)
		|| F.doesNotAccessMemory()
		|| F.isDeclaration()
		|| HasFunctionName(F, LOOP_TRIPCOUNT)
		)
		return true;
	
	return false;
}


//Check if an instruction is a Function call and has energy annotation
bool Analysis::HasEnergyAnnotation(const llvm::Instruction& I) const
{

	const llvm::Function* fn = IsFunction(I);
	if (fn == nullptr)
		return false;

	return HasEnergyAnnotation(*fn);
}


//Check if a Function has energy annotation
//overloaded function of instruction
bool Analysis::HasEnergyAnnotation(const llvm::Function& F) const
{
	if (F.hasFnAttribute(ENERGY_ATTR))
		return true;

	return false;
}

// Method to find the entry point
// At this point a fixed value in AnalysisVisitor.h
// Looks up the main entry point by name and returns a pointer to it.
llvm::Function* Analysis::GetModuleEntryPoint(llvm::Module& M) const
{

	for (llvm::Function& F : M)
	{
		if (F.getName().str() == MODULE_ENTRY_POINT)
		{
			return &F;
		}
	}

	return nullptr;
}