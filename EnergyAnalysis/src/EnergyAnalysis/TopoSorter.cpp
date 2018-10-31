#include "TopoSorter.h"

void TopoSorter::visit(EnergyModule & em)
{

	llvm::Module& M = em.GetLLVMModule();

	for (llvm::Function& F : M)
	{

		if (F.hasFnAttribute("Energy"))
		{
			llvm::outs() << F.getFnAttribute("name").getValueAsString().str() << " " << F.getFnAttribute("t").getValueAsString().str() << "\n";
		}
		//runToposort(F);
	}
	
}

void TopoSorter::runToposort(llvm::Function& F)
{
	llvm::outs() << "Topological sort of " << F.getName().str() << ":\n";

	
	// Colormap all vertices start white
	/*for (llvm::Function::const_iterator I = F.begin(), IE = F.end(); I != IE; I++)
	{
		ColorMap[&*I] = TopoSorter::WHITE;
	}*/

	//print no of BB
	int bbC = 0;
	int iC = 0;
	for (llvm::BasicBlock& BB : F) {
		// Colormap all vertices start white
		ColorMap[&BB] = TopoSorter::WHITE;
		bbC++; //set the counter
		for (llvm::Instruction& Instr : BB)
		{
			
			iC++;
			

			
			//llvm::errs() << Instr << "\n";
		}
	}
	llvm::outs() << "Number of BBs: " << std::to_string(bbC) << "\n";
	llvm::outs() << "Number of Instructions: " << std::to_string(iC) << "\n";


	//When our function has 0 BB's and 0 Instructions it is probably a dummy function
	
	if (bbC < 2) {
		
		if (iC > 0)
		{
			llvm::outs() << "Function with Instructions only" << "\n";
		}
		else
		{
			llvm::outs() << "Empty function declared outside of translation unit" << "\n";
		}
		
		return;
	}

	bool success = recursiveDFSToposort(&F.getEntryBlock());
	if (success) {
		for (BBVector::const_reverse_iterator RI = SortedBBS.rbegin(), RE = SortedBBS.rend(); RI != RE; ++RI)
		{
			if (!(*RI)->getName().empty())
			{

				llvm::outs() << " " << getSimpleNodeLabel(const_cast<llvm::BasicBlock*>(*RI)) << "\n";
				//llvm::BasicBlock* BB = const_cast<llvm::BasicBlock*>(*RI);
				/*for (llvm::Instruction& Instr : *BB) {
					llvm::errs() << Instr << "\n";
				}*/
			}
			
				
			
			

		}
	}
	else
	{
		llvm::outs() << " Sorting failed" << "\n";
	}
	

}

bool TopoSorter::recursiveDFSToposort(llvm::BasicBlock * BB)
{
	ColorMap[BB] = TopoSorter::GREY;

	// Get all terminators for this basic block
	const llvm::TerminatorInst  *TInst = BB->getTerminator();
	unsigned int NSucc = 0;
	if (TInst != nullptr)
	{
		NSucc = TInst->getNumSuccessors();
	}
	
	
	llvm::outs() << " Name of BB: " << getSimpleNodeLabel(BB) << "\n";
	CheckForFunctions(*BB);

	
	for (unsigned I = 0; I < NSucc; ++I) 
	{
		//loop through all terminators
		llvm::BasicBlock *Succ = TInst->getSuccessor(I);
		Color SuccColor = ColorMap[Succ];
		if (SuccColor == TopoSorter::WHITE)
		{
			if (!recursiveDFSToposort(Succ))
				return false;
		}
		else if (SuccColor = TopoSorter::GREY)
		{
			//we have a cycle
			llvm::outs() << " Detected cycle: edge from  " << getSimpleNodeLabel(BB) << " to " << getSimpleNodeLabel(Succ) << "\n";
			
			llvm::outs() << "\n";
		}
	}
	ColorMap[BB] = TopoSorter::BLACK;
	SortedBBS.push_back(BB);
	return true;
}

void TopoSorter::CheckForFunctions(llvm::BasicBlock& BB)
{
	for (llvm::Instruction& inst : BB)
	{
		//inst.dump();
		llvm::CallInst * callInst = llvm::dyn_cast<llvm::CallInst>(&inst);
	   
		if (callInst == nullptr)
			continue;

		//callInst->dump();
		if (llvm::Function *calledFunction = callInst->getCalledFunction()) {
			std::cout << "Call to => " << calledFunction->getName().str() << std::endl;
			if (calledFunction->hasFnAttribute("Energy"))
			{
				std::cout << calledFunction->getFnAttribute("name").getValueAsString().str() << " pd="
					<< calledFunction->getFnAttribute("pd").getValueAsString().str() << " e=c"
					<< calledFunction->getFnAttribute("ec").getValueAsString().str() << " t="
					<< calledFunction->getFnAttribute("t").getValueAsString().str()
					<< std::endl;
			}
		}
		else
		{
			llvm::Type* t = callInst->getCalledValue()->getType();
			if (llvm::FunctionType* ft = llvm::cast<llvm::FunctionType>(llvm::cast<llvm::PointerType>(t)->getElementType()))
			{
				if (llvm::Value *calledVoid = callInst->getCalledValue())
				{
					llvm::Function* voidFunc = llvm::dyn_cast_or_null<llvm::Function>(calledVoid);
					if (voidFunc != nullptr)
						std::cout << calledFunction->getFnAttribute("name").getValueAsString().str() << std::endl;
				}
			}
			
			
		}

		

		
	}
}

std::string TopoSorter::getSimpleNodeLabel( llvm::BasicBlock* Node)
{
	

	if (!Node->getName().empty())
		return Node->getName().str();

	std::string Str;
	llvm::raw_string_ostream OS(Str);

	Node->printAsOperand(OS, false);
	return OS.str();
}



