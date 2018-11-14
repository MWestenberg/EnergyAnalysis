#include "CallGraphVisitor.h"



void CallGraphVisitor::visit(EnergyModule& em)
{
	llvm::Module& module = em.GetLLVMModule();
	
	//llvm::CallGraph cfg = llvm::CallGraph(module);
	//cfg.dump();

	// This just gets the main entry point from the Module.
	llvm::Function *mainFunction = GetModuleEntryPoint(module);

	TraverseFunction(*mainFunction);

	//Create vector of ordered basic blocks per function
	//Only on functions that are not external components
	SortBasicBlocks(module);
	
	// Get the list of basic blocks for the Main function
	//OrderedBB bbs = OrderedF[mainFunction];
	
	//IterateFunction(bbs, true);


	//for (llvm::BasicBlock* b : bbs)
	//{
	//	std::cout << getSimpleNodeLabel(b) << std::endl;
	//	//Traverse the instructions per Basic block
	//	//TraverseInstructions(*b);
	//	
	//}
}



// The main function is a C/C++ entry point. LLVM is not aware of the real module entry point.
// TODO: allow annotating the entry function 
// for now look for main function
llvm::Function* CallGraphVisitor::GetModuleEntryPoint(llvm::Module& M)
{

	for (llvm::Function& F : M)
	{
		if (F.getName().str() == "main")
		{
			return &F;
		}
	}

	return nullptr;
}


bool CallGraphVisitor::HasFunctionName(const llvm::Function& F, const llvm::StringRef& name) const
{
	if (F.getName() == name)
		return true;

	return false;
}


//Check if the instruction is a function. return nullptr in case it is not a function else a function pointer
llvm::Function* CallGraphVisitor::IsFunction(llvm::Instruction& I) const
{

	llvm::CallInst * callInst = llvm::dyn_cast<llvm::CallInst>(&I);
	if (callInst == nullptr)
		return nullptr;

	if (llvm::Function *tailCall = llvm::dyn_cast<llvm::Function>(callInst->getCalledValue()->stripPointerCasts()))
	{
		std::cout << "   ===> Call to Function => " << tailCall->getName().str();
		return tailCall;
	}
		

	return nullptr;

}


//Check if an instruction is a Function call and has energy annotation
bool CallGraphVisitor::HasEnergyAnnotation(llvm::Instruction& I) const
{

	const llvm::Function* fn = IsFunction(I);
	if (fn == nullptr)
		return false;

	return HasEnergyAnnotation(*fn);
}


//Check if a Function has energy annotation
//overloaded function of instruction
bool CallGraphVisitor::HasEnergyAnnotation(const llvm::Function& F) const
{
	if (F.hasFnAttribute("Energy"))
		return true;

	return false;
}


void CallGraphVisitor::IterateFunction(OrderedBB& F, bool firstRun)
{
	for (llvm::BasicBlock* BB : F)
	{
		for (llvm::Instruction& inst : *BB)
		{
			inst.dump();

			//retrieve call instruction
			llvm::Function* fn = IsFunction(inst);
		
			//First run means we can ignore the non energy functions
			if (firstRun && fn == nullptr) {
				
				llvm::outs() << " (first run: ignored)" << "\n\n";
				continue; //no need to calc instruction cost
			}
			
			if (fn != nullptr && HasFunctionName(*fn, LOOP_TRIPCOUNT))
			{
				int tripCount = GetLoopTripCount(inst);
				llvm::outs() << " (Loop start)" << "\n\n";

				
				continue;
			}
		
			if (firstRun && fn != nullptr)
			{
				//Check for Energy  Functions
				if (HasEnergyAnnotation(*fn))
				{
				
					llvm::outs() << " (First Energy Function)" << "\n\n";
					//calc the first energy function
					firstRun = false;
					continue;
				}
			
				//Normal function so traverse this function recursively
				llvm::outs() << " (Normal function needs traversing" << "\n\n";

			}
			else
			{

				//In case of first run, continue until a function is found
				if (firstRun) {
					llvm::outs() << " (first run: ignored)" << "\n\n";
					continue;
				}
					   
				if (fn != nullptr)
				{
					//Check for Energy  Functions
					if (HasEnergyAnnotation(*fn))
					{
						llvm::outs() << " (Follow-up Energy Function)" << "\n\n";
						//calc the first energy function
						firstRun = false;
					
					
						continue;
					}

					
					//Normal function so traverse this function recursively
					llvm::outs() << " (Normal function needs traversing" << "\n\n";
					
				}
				else
				{
					llvm::outs() << " (normal instruction)" << "\n\n";
					//normal instruction need to calc cost
				}


			}


		}
	}
}


// Iterates the entire module and looks for functions that are declared
// Each function is then traversed and all Basic Blocks are sorted and added to OrderedF
// Energy functions are omitted
void  CallGraphVisitor::SortBasicBlocks(llvm::Module& M)
{

	for (llvm::Function& F : M)
	{
		
		//Energy Functions are not traversed. 
		//Functions that cannot be accessed can be ignored
		if (HasEnergyAnnotation(F) || F.doesNotAccessMemory())
			continue;

		
		//TODO: this is now hardcoded must be dynamic
		//Could not find how to analyze a function that is not declared.
		if (HasFunctionName(F, LOOP_TRIPCOUNT))
			continue;

		OrderedBB orderedBB;
		for (llvm::po_iterator<llvm::BasicBlock*> I = llvm::po_begin(&F.getEntryBlock()),
			IE = llvm::po_end(&F.getEntryBlock()); I != IE; I++)
		{
			orderedBB.push_back(*I);
		}

		//reverse order
		std::reverse(orderedBB.begin(), orderedBB.end());

		//assign to vector
		OrderedF[&F] = orderedBB;


		//TO DO: Loops
	}

}

int CallGraphVisitor::GetLoopTripCount(llvm::Instruction& I) const
{
	llvm::CallInst * callInst = llvm::dyn_cast<llvm::CallInst>(&I);
	llvm::ConstantInt* tripCountConstant = llvm::dyn_cast<llvm::ConstantInt>(callInst->getOperand(0));
	int64_t tripCount = tripCountConstant->getSExtValue();
	return tripCount;
}


/*======================================================================================================================

	BELOW THIS POINT ARE TEST FUNCTIONS

=======================================================================================================================*/
//Although this traverses the instructions it is only looking for function calls.
//Final iteration will be done somewhere else. 
void CallGraphVisitor::TraverseInstructions(llvm::BasicBlock& BB)
{


	for (llvm::Instruction& inst : BB)
	{
		llvm::CallInst * callInst = llvm::dyn_cast<llvm::CallInst>(&inst);

		if (callInst == nullptr)
			continue;

		std::cout << " ";
		inst.dump();

		//Check if the instruction is a function call
		if (llvm::Function *tailCall = llvm::dyn_cast<llvm::Function>(callInst->getCalledValue()->stripPointerCasts()))
		{
			std::cout << "   ===> Call to Function => " << tailCall->getName().str();

			if (tailCall->getName() == LOOP_TRIPCOUNT)
			{
				llvm::ConstantInt* tripCountConstant = llvm::dyn_cast<llvm::ConstantInt>(callInst->getOperand(0));
				int64_t tripCount = tripCountConstant->getSExtValue();

				std::cout << "Basic block " << getSimpleNodeLabel(&BB) << " is part of a LOOP with trip count " << tripCount << std::endl;
				

				continue;
			}

			//Check for Energy  Functions
			if (tailCall->hasFnAttribute("Energy"))
			{
				// Get the Name
				llvm::StringRef eName = tailCall->getFnAttribute("name").getValueAsString();
				//Check if name was already used
				auto found = m_eFunctions.find(eName);

				//if used power draw must stop
				if (found != m_eFunctions.end())
				{
					std::cout << "  => EName: " << eName.str() << " STOP powerdraw (" << found->second.str() << ")" << std::endl;
				}
				else
				{
					//only add to vector when powerdraw is greater than 0. Else it is just time
					unsigned int pd = std::stoi(tailCall->getFnAttribute("pd").getValueAsString().str());
					if (pd > 0)
						m_eFunctions.insert(std::pair<llvm::StringRef, llvm::StringRef>(eName, tailCall->getFnAttribute("pd").getValueAsString()));


					std::cout << " => EName: " << eName.str()
						<< " (pd="
						<< tailCall->getFnAttribute("pd").getValueAsString().str() << " ec="
						<< tailCall->getFnAttribute("ec").getValueAsString().str() << " t="
						<< tailCall->getFnAttribute("t").getValueAsString().str() << ")"
						<< std::endl;
				}
			}

		}
	}
}

void CallGraphVisitor::LoopCountTest(llvm::Function & F)
{

	llvm::DominatorTree* DT = new llvm::DominatorTree();
	DT->recalculate(F);
	llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>* KLoop = new llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>();
	KLoop->releaseMemory();
	KLoop->analyze(*DT);

	for (llvm::BasicBlock& BB : F)
	{
		const llvm::Loop* L = KLoop->getLoopFor(&BB);
		if (L != NULL)
		{
			L->dump();
			llvm::BasicBlock* h = L->getHeader();

			std::cout << L->getNumBackEdges() << std::endl;

			if (llvm::BranchInst *bi = llvm::dyn_cast<llvm::BranchInst>(h->getTerminator())) {
				llvm::Value *loopCond = bi->getCondition();
				loopCond->dump();
			}

		}
	}
	
	
}

//This traveses the function 
void  CallGraphVisitor::TraverseFunction(llvm::Function& F)
{	
	OrderedBB orderedBB;
	for (llvm::po_iterator<llvm::BasicBlock*> I = llvm::po_begin(&F.getEntryBlock()),
		IE = llvm::po_end(&F.getEntryBlock()); I != IE; I++)
	{
		orderedBB.push_back(*I);
	}

	//reverse order
	std::reverse(orderedBB.begin(), orderedBB.end());
	
	//assign to vector
	OrderedF[&F] = orderedBB;

	//Handle SCC's (Loops)
	for (llvm::scc_iterator<llvm::Function *> I = llvm::scc_begin(&F),
		IE = llvm::scc_end(&F);
		I != IE; ++I)
	{

		OrderedBB &SCCBBs = const_cast<OrderedBB&>(*I); //cast to non const
		std::reverse(SCCBBs.begin(), SCCBBs.end()); //reverse the order
		//SCCs[&F] = SCCBBs;

		
		llvm::outs() << "  SCC: ";

		if (SCCBBs.size() == 1)
		{
			llvm::outs() << getSimpleNodeLabel(SCCBBs[0]) << " (no loop)\n";
			continue;
		}

		OrderedBB tmp;

		for (std::vector<llvm::BasicBlock *>::const_iterator BBI = SCCBBs.begin(),
			BBIE = SCCBBs.end();
			BBI != BBIE; ++BBI) 
		{
			tmp.push_back(*BBI);

			llvm::outs() << getSimpleNodeLabel(*BBI) << "  ";

			const llvm::BasicBlock* BB = *BBI;
			const llvm::TerminatorInst *TInst = BB->getTerminator();
			for (unsigned I = 0, NSucc = TInst->getNumSuccessors(); I < NSucc; ++I) {
				llvm::BasicBlock *Succ = TInst->getSuccessor(I);
				
				if (std::find(tmp.begin(), tmp.end(), Succ) != tmp.end())
				{
					llvm::outs() << "(nested loop to  " << getSimpleNodeLabel(Succ) << ") ";
				}
				
			}

		}
		llvm::outs() << "\n";
	}
}








//TODO: clean this up
void CallGraphVisitor::PostOrderTraversal(llvm::Module& M)
{

	for (llvm::Function& F : M)
	{
		llvm::outs() << "Basic blocks of " << F.getName() << " in post-order:\n";
		for (llvm::po_iterator<llvm::BasicBlock*> I = llvm::po_begin(&F.getEntryBlock()),
			IE = llvm::po_end(&F.getEntryBlock()); I != IE; I++)
		{
			
		}
	}

	/*std::reverse(vec.begin(), vec.end());
	for (auto &B : vec)
	{
		llvm::outs() << " " << getSimpleNodeLabel(B) << "\n";
	}*/

}





//llvm::SmallVector<llvm::CallGraphNode *, 16> Nodes;
	//Nodes.reserve(FunctionMap.size());




	//each node can be printed via CallGraphNode.print(OS);


	//llvm::raw_ostream * out = &llvm::outs();
	//cfg.print(*out);


	//for (std::map<const llvm::Function *, std::unique_ptr<llvm::CallGraphNode>>::iterator it = cfg.begin(); it != cfg.end(); ++it)
	//{
	/*	if (it->first != nullptr)
		{
			std::cout << it->first << " => " << it->second << std::endl;

			const llvm::Function& F = *it->first;
			llvm::StringRef fName = F.getName();
			std::cout << fName.str() << std::endl;

		}*/
		//}
