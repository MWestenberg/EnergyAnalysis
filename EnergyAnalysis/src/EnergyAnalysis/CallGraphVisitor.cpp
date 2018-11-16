#include "CallGraphVisitor.h"



void CallGraphVisitor::visit(EnergyModule& em)
{
	llvm::Module& module = em.GetLLVMModule();
	
	//llvm::CallGraph cfg = llvm::CallGraph(module);
	//cfg.dump();

	// This just gets the main entry point from the Module.
	llvm::Function *mainFunction = GetModuleEntryPoint(module);

	//TraverseFunction(*mainFunction);

	//Create vector of ordered basic blocks per function
	//Only on functions that are not external components
	SortBasicBlocks(module);
	
	

	// Get the list of basic blocks for the Main function
	OrderedBB bbs = OrderedF[mainFunction];
	
	//Print(bbs);
	//PrintAllLoops();


	IterateFunction(bbs, true);


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
		if (F.getName().str() == MODULE_ENTRY_POINT)
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
		// log.LogInfo("   ===> Call to Function => " +  tailCall->getName().str() + "\n")
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
	if (F.hasFnAttribute(ENERGY_ATTR))
		return true;

	return false;
}


void CallGraphVisitor::IterateFunction(OrderedBB& OBB, bool firstRun)
{
	for (llvm::BasicBlock* BB : OBB)
	{

		log.LogInfo("========================================"  + getSimpleNodeLabel(BB) + "========================================");
		for (llvm::Instruction& inst : *BB)
		{
			if (log.GetLevel() == log.INFO)
				inst.dump();

			//retrieve call instruction
			llvm::Function* fn = IsFunction(inst);
		
			//First run means we can ignore the non energy functions
			if (firstRun && fn == nullptr) {
				log.LogInfo(" (first run: ignored) \n\n");
				continue; //no need to calc instruction cost
			}

		
			if (firstRun && fn != nullptr)
			{
				//Check for Energy  Functions
				if (HasEnergyAnnotation(*fn))
				{
				
					log.LogInfo(" (First Energy Function) \n\n");
					//calc the first energy function
					firstRun = false;

					continue;
				}
			
				//Normal function so traverse this function recursively
				//calc cost of tail call first
				// then traverse function
				log.LogInfo(" (Normal function needs traversing \n\n");
				IterateFunction(OrderedF[fn], firstRun);
			}
			else
			{

				//In case of first run, continue until a function is found
				if (firstRun) {
					log.LogInfo(" (first run: ignored) \n\n");
					continue;
				}
					   
				if (fn != nullptr)
				{
					//Check for Energy  Functions
					if (HasEnergyAnnotation(*fn))
					{
						log.LogInfo(" (Follow-up Energy Function) \n\n");
						//calc the first energy function
						firstRun = false;
					
					
						continue;
					}

					
					//Normal function so traverse this function recursively
					//calc cost of tail call first
					// then traverse function
					log.LogInfo(" (Normal function needs traversing \n\n");
					IterateFunction(OrderedF[fn], firstRun);
					
				}
				else
				{
					log.LogInfo(" (normal instruction) \n\n");
					//normal instruction need to calc cost

					//The last instruction of a basic Block is always of type br meaning it is a normal instruction
					//check if the current instruction is the last instruction of the basic block
					//also check if it is a loop
					if (BB->getTerminator() == &inst && IsLoopStart(*BB))
					{
						log.LogInfo("========================================LOOP START===============================\n");
						//When entered here the current Basic Block is the start of a loop
						LoopOrderedSet& currentLoop = GetLoop(*BB);
						currentLoop.currentTrip++;
						if (currentLoop.tripCount >= currentLoop.currentTrip)
						{
							log.LogInfo("COUNTER: " + std::to_string(currentLoop.currentTrip) + " / " + std::to_string(currentLoop.tripCount) + "\n");
							IterateFunction(currentLoop.loop, firstRun);
						}
						else
						{
							currentLoop.currentTrip = 1;
							log.LogInfo("========================================LOOP END=================================\n");
						}

						

					}
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
		log.LogInfo("Function:  " + F.getName().str() + "\n");
		
		//Energy Functions are not traversed. 
		//Functions that cannot be accessed can be ignored
		if (HasEnergyAnnotation(F) || F.doesNotAccessMemory())
			continue;
				
		//TODO: this is now hardcoded must be dynamic
		//Could not find how to analyze a function that is not declared.
		if (HasFunctionName(F, LOOP_TRIPCOUNT))
			continue;

		//Create a ordered set of Basic Blocks
		OrderedBB orderedBB;
		//Depth First traversal of Basic Blocks
		for (llvm::po_iterator<llvm::BasicBlock*> I = llvm::po_begin(&F.getEntryBlock()),
			IE = llvm::po_end(&F.getEntryBlock()); I != IE; I++)
		{
			orderedBB.push_back(*I);
		}

		// Reverse order so it is in topological order
		std::reverse(orderedBB.begin(), orderedBB.end());

		//assign to member vector
		OrderedF[&F] = orderedBB;

		//Handle Loops using SCC iterator
		for (llvm::scc_iterator<llvm::Function *> I = llvm::scc_begin(&F),
			IE = llvm::scc_end(&F);
			I != IE; ++I)
		{
			OrderedBB &SCCBBs = const_cast<OrderedBB&>(*I); //cast to non const
			std::reverse(SCCBBs.begin(), SCCBBs.end());
			//if (SCCBBs.size() == 1)
			//	log.LogInfo("Not part of a loop: "+ getSimpleNodeLabel(SCCBBs[0]) + "\n");
			
			LoopLookup(SCCBBs);
	
		}

	}

}

void CallGraphVisitor::LoopLookup(OrderedBB& SCCBBs)
{
	//Create temporary Set
	LoopOrderedSet los;

	//Iterate the basic block and check if it contains a nested loop
	for (std::vector<llvm::BasicBlock *>::const_iterator BBI = SCCBBs.begin(),
		BBIE = SCCBBs.end();
		BBI != BBIE; ++BBI)
	{
		//store it to check later if it is referenced to
		los.loop.push_back(*BBI);

		// make Basic Block a const pointer
		const llvm::BasicBlock* BB = *BBI;
		// Get the termination instruction of the basic block
		const llvm::TerminatorInst *TInst = BB->getTerminator();
		//Iterate over all successors of the Basic block
		for (unsigned I = 0, NSucc = TInst->getNumSuccessors(); I < NSucc; ++I) {
			llvm::BasicBlock *Succ = TInst->getSuccessor(I);

			// This returns true when the Successor terminator was already added to the temporay loops
			// this works because we order the Basic Blocks in topological order for each function
			if (std::find(los.loop.begin(), los.loop.end(), Succ) != los.loop.end())
			{
				log.LogInfo("Loop from BB: " + getSimpleNodeLabel(Succ) + "  loop to BB: " + getSimpleNodeLabel(*BBI) + "\n");
				LoopOrderedSet nestedLoop;
				CreateLoopSet(los, nestedLoop, Succ, *BBI);
				LoopSet[*BBI] = nestedLoop;
				continue;

			}
		}

	}
}

LoopOrderedSet& CallGraphVisitor::GetLoop(llvm::BasicBlock& BB)
{
	assert(IsLoopStart(BB) && "Passed Basic Block must be a start of a loop and part of LoopSet");
	return LoopSet[&BB];

}

bool CallGraphVisitor::IsLoopStart(llvm::BasicBlock& BB)
{
	auto found = LoopSet.find(&BB);
	if (found != LoopSet.end())
		return true;
		

	return false;

}


void CallGraphVisitor::SetLoopTripCount(LoopOrderedSet& ls)
{

	for (llvm::BasicBlock* BB : ls.loop)
	{
		for (llvm::Instruction& inst : *BB)
		{
			llvm::Function* fn = IsFunction(inst);
			if (fn != nullptr && HasFunctionName(*fn, LOOP_TRIPCOUNT))
			{
				int tripCount = GetLoopTripCount(inst);
				ls.tripCount = tripCount;
				inst.eraseFromParent();
				break;
			}
		}
	}

}

void CallGraphVisitor::CreateLoopSet(LoopOrderedSet& stack, LoopOrderedSet& nestedLoop, llvm::BasicBlock* BBbegin, llvm::BasicBlock* BBend)
{
	log.LogInfo(" LOOP: " + getSimpleNodeLabel(BBbegin) + "\n");
	int i = 0;
	for (llvm::BasicBlock* BB : stack.loop)
	{
		if (BB == BBbegin && i==0)
		{
			nestedLoop.loop.push_back(BB);
			SetLoopTripCount(nestedLoop);
			i++;
			nestedLoop.loopSize = i;
		} else if (i > 0)
		{

			nestedLoop.loop.push_back(BB);
			i++;
			if (BB == BBend)
			{
				nestedLoop.loopSize = i;
				break;
			}
				
		}
	}
	
	
}

int CallGraphVisitor::GetLoopTripCount(llvm::Instruction& I) const
{
	llvm::Function* fn = IsFunction(I);
	assert((fn != nullptr && HasFunctionName(*fn, LOOP_TRIPCOUNT)) && "The passed instruction must a function and have a name defined as LOOP_TRIPCOUNT and a integer as first parameter");

	llvm::CallInst * callInst = llvm::dyn_cast<llvm::CallInst>(&I);
	llvm::ConstantInt* tripCountConstant = llvm::dyn_cast<llvm::ConstantInt>(callInst->getOperand(0));
	int64_t tripCount = tripCountConstant->getSExtValue();
	return tripCount;
}




/*======================================================================================================================

	BELOW THIS POINT ARE TEST FUNCTIONS

=======================================================================================================================*/

//LoopOrderedSet GetNestedLoop(LoopOrderedSet& stack, const llvm::BasicBlock& BB)
//{
//	LoopOrderedSet newLNestedLoop;
//
//	//find terminator
//	const llvm::TerminatorInst *TInst = BB.getTerminator();
//	for (unsigned I = 0, NSucc = TInst->getNumSuccessors(); I < NSucc; ++I) {
//		llvm::BasicBlock *Succ = TInst->getSuccessor(I);
//
//		//get successor and lookup in stack if it already exists
//		if (std::find(stack.loop.begin(), stack.loop.end(), Succ) != stack.loop.end())
//		{
//			//When entered here we have a nested loop to BasicBlock Succ
//
//			//llvm::outs() << "(nested loop to  " << getSimpleNodeLabel(Succ) << ") ";
//		}
//
//	}
//
//	return newLNestedLoop;
//}

//Although this traverses the instructions it is only looking for function calls.
//Final iteration will be done somewhere else. 
void CallGraphVisitor::TraverseInstructions(llvm::BasicBlock& BB)
{


	for (llvm::Instruction& inst : BB)
	{
		llvm::CallInst * callInst = llvm::dyn_cast<llvm::CallInst>(&inst);

		if (callInst == nullptr)
			continue;

		log.LogInfo(" ");
		inst.dump();

		//Check if the instruction is a function call
		if (llvm::Function *tailCall = llvm::dyn_cast<llvm::Function>(callInst->getCalledValue()->stripPointerCasts()))
		{
			log.LogInfo("   ===> Call to Function => " + tailCall->getName().str());

			if (tailCall->getName() == LOOP_TRIPCOUNT)
			{
				llvm::ConstantInt* tripCountConstant = llvm::dyn_cast<llvm::ConstantInt>(callInst->getOperand(0));
				int64_t tripCount = tripCountConstant->getSExtValue();

				log.LogInfo("Basic block " + getSimpleNodeLabel(&BB) + " is part of a LOOP with trip count " + std::to_string(tripCount) + "\n");
				

				continue;
			}

			//Check for Energy  Functions
			if (tailCall->hasFnAttribute(ENERGY_ATTR))
			{
				// Get the Name
				llvm::StringRef eName = tailCall->getFnAttribute(ENERGY_FUNCTION_NAME).getValueAsString();
				//Check if name was already used
				auto found = m_eFunctions.find(eName);

				//if used power draw must stop
				if (found != m_eFunctions.end())
				{
					log.LogInfo("  => EName: " + eName.str() + " STOP powerdraw (" + found->second.str() + ")" + "\n");
				}
				else
				{
					//only add to vector when powerdraw is greater than 0. Else it is just time
					unsigned int pd = std::stoi(tailCall->getFnAttribute("pd").getValueAsString().str());
					if (pd > 0)
						m_eFunctions.insert(std::pair<llvm::StringRef, llvm::StringRef>(eName, tailCall->getFnAttribute("pd").getValueAsString()));


					log.LogInfo(" => EName: " + eName.str()
						+ " (pd="
						+ tailCall->getFnAttribute(ENERGY_TEMPORAL_CONSUMPTION).getValueAsString().str() + " ec="
						+ tailCall->getFnAttribute(ENERGY_CONSUMPTION).getValueAsString().str() + " t="
						+ tailCall->getFnAttribute(ENERGY_TIME_UNIT).getValueAsString().str() + ")"
						+ "\n");
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

			log.LogInfo(L->getNumBackEdges() + "\n");

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
			
		log.LogInfo("  SCC: ");

		if (SCCBBs.size() == 1)
		{
			log.LogInfo(getSimpleNodeLabel(SCCBBs[0]) + " (no loop)\n");
			continue;
		}

			
		LoopOrderedSet los;

		for (std::vector<llvm::BasicBlock *>::const_iterator BBI = SCCBBs.begin(),
			BBIE = SCCBBs.end();
			BBI != BBIE; ++BBI) 
		{
			los.loop.push_back(*BBI);

			log.LogInfo(getSimpleNodeLabel(*BBI) + "  ");

			const llvm::BasicBlock* BB = *BBI;
			const llvm::TerminatorInst *TInst = BB->getTerminator();
			for (unsigned I = 0, NSucc = TInst->getNumSuccessors(); I < NSucc; ++I) {
				llvm::BasicBlock *Succ = TInst->getSuccessor(I);
				
				if (std::find(los.loop.begin(), los.loop.end(), Succ) != los.loop.end())
				{
					log.LogInfo("(nested loop to  " + getSimpleNodeLabel(Succ) + ") ");
				}
				
			}

		}
		log.LogInfo("\n");
		
		los.tripCount = los.loop.size();
		LoopSet[SCCBBs[0]] = los;


	}
}








//TODO: clean this up
void CallGraphVisitor::PostOrderTraversal(llvm::Module& M)
{

	for (llvm::Function& F : M)
	{
		log.LogInfo("Basic blocks of " + F.getName().str() + " in post-order:\n");
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

void CallGraphVisitor::PrintAllLoops()
{
	
	for (llvm::DenseMap<llvm::BasicBlock*, LoopOrderedSet>::iterator IT = LoopSet.begin(),
		IE = LoopSet.end(); IT != IE; IT++)
	{
		log.LogInfo(getSimpleNodeLabel(IT->first) + ": ");
		LoopOrderedSet set = IT->second;

		log.LogInfo("(" + std::to_string(set.tripCount) + ", "+ std::to_string(set.loopSize) + "): ");

		for (llvm::BasicBlock* bb : set.loop)
		{
			log.LogInfo(getSimpleNodeLabel(bb));
		}
		
		log.LogInfo("\n");
	}

}

void CallGraphVisitor::Print(OrderedBB & F)
{
	for (llvm::BasicBlock* bb : F)
	{
		log.LogInfo(getSimpleNodeLabel(bb));
		auto x = LoopSet.find(bb);
		if (x != LoopSet.end())
		{
			//has a loop
			for (llvm::BasicBlock* b : LoopSet[bb].loop)
			{
			  log.LogInfo(getSimpleNodeLabel(b) + ", ");
			}

		}
			

		log.LogInfo("\n");
	}
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
