#include "CallGraphVisitor.h"



void CallGraphVisitor::visit(EnergyModule& em)
{
	llvm::Module& module = em.GetLLVMModule();
	
	//llvm::CallGraph cfg = llvm::CallGraph(module);
	//cfg.dump();

	// This just gets the main entry point from the Module.
	llvm::Function *mainFunction = GetModuleEntryPoint(module);

	//Create vector of ordered basic blocks per function
	//Only on functions that are not external components
	SortBasicBlocks(module);
	
	// Get the list of basic blocks for the Main function
	OrderedBB entryPointBBs = OrderedF[mainFunction];
	
	//	llvm::raw_ostream *out = &llvm::outs();
	

	//Print(bbs);
	//PrintAllLoops();
	IterateFunction(entryPointBBs);
	//C:\Dev\LLVM-BUILD\Release\bin\opt.exe -analyze -targetlibinfo -cost-model C:\Dev\EnergyAnalysis\examples\plateCutter.bc
	

	
	
	


	

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
		// log.LogDebug("   ===> Call to Function => " +  tailCall->getName().str() + "\n")
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

void  CallGraphVisitor::SetInstructionCost(const llvm::Instruction& I)
{
	
	std::unique_ptr<WCETCostModelAnalysis> wcet(new WCETCostModelAnalysis);

	const llvm::BasicBlock* BB = I.getParent();
	llvm::Function* parentFunction = const_cast<llvm::Function*>(BB->getParent());
	wcet->runOnFunction(*parentFunction);
	unsigned int cost = wcet->getInstructionCost(&I);
	log.LogInfo(" cost: " + std::to_string(cost) + "\n");

}

bool  CallGraphVisitor::IsFirstEncounter(const llvm::Function& F)
{

	auto found = EnergyFunctionList.find(&F);
	if (found != EnergyFunctionList.end())
		return true;
	return false;
}

bool  CallGraphVisitor::SetEnergyFunctionCost(llvm::Function& F)
{
	if (&F != nullptr && HasEnergyAnnotation(F))
	{
		//Set first run to false;
		if (m_firstRun)
			m_firstRun = false;

		llvm::StringRef name = "";
		signed int pd = 0;
		unsigned int ec = 0;
		unsigned int t = 0;

		if (F.hasFnAttribute(ENERGY_FUNCTION_NAME)) 
		{
			name = F.getFnAttribute(ENERGY_FUNCTION_NAME).getValueAsString();
			if (F.hasFnAttribute(ENERGY_TEMPORAL_CONSUMPTION))
			{
				pd = std::stoi(F.getFnAttribute(ENERGY_TEMPORAL_CONSUMPTION).getValueAsString().str());
			}

			//TODO: Calculation of energy function powerdraw and single consumption

			// below is not ready please finish.
			if (IsFirstEncounter(F)) {
				EnergyFunction ef;
				ef[&F] = pd;
				// add the powerdraw to Joules counter and to the powerdraw counter
			}
			else
			{
				
					
			}

		}
			

		if (F.hasFnAttribute(ENERGY_FUNCTION_NAME))
			pd = std::stoi(F.getFnAttribute(ENERGY_TEMPORAL_CONSUMPTION).getValueAsString().str());

		if (F.hasFnAttribute(ENERGY_CONSUMPTION))
			ec = std::stoi(F.getFnAttribute(ENERGY_CONSUMPTION).getValueAsString().str());

		if (F.hasFnAttribute(ENERGY_TIME_UNIT))
			t = std::stoi(F.getFnAttribute(ENERGY_TIME_UNIT).getValueAsString().str());

		log.LogInfo(F.getName().str() + " has Energy values: Time-dependent consumption=" + std::to_string(pd) + " One-time energy consumption=" + std::to_string(ec) + " Time=" + std::to_string(t) + "!\n");

		return true;
	}
	return false;
}

//This methods is the main iterator. It uses all collected data
// and traverses the ordered set of basic blocks of each function that it 
// it takes as parameter
// first run should be true on firstrun after that it is no longer required
void CallGraphVisitor::IterateFunction(OrderedBB& OBB)
{
	for (llvm::BasicBlock* BB : OBB)
	{		log.LogDebug("========================================"  + getSimpleNodeLabel(BB) + "========================================\n");
		for (llvm::Instruction& inst : *BB)
		{
			if (log.GetLevel() == log.DEBUG)
				inst.dump();
			
			//retrieve call instruction
			llvm::Function* fn = IsFunction(inst);
		
					   			 			//First run means we can ignore the non energy functions
			if (m_firstRun && fn == nullptr) {
				log.LogDebug(" (first run: ignored) \n\n");
				continue; //no need to calc instruction cost
			}


			if (m_firstRun && fn != nullptr)
			{
				if (SetEnergyFunctionCost(*fn))
				{
					log.LogDebug(" (First Energy Function) \n\n");
					continue;
				}

				//Normal function so traverse this function recursively
				//calc cost of tail call first
				// then traverse function
				log.LogDebug(" (Normal function needs traversing \n\n");
				IterateFunction(OrderedF[fn]);
			}
			else
			{

				//In case of first run, continue until a function is found
				if (m_firstRun) {
					log.LogDebug(" (first run: ignored) \n\n");
					continue;
				}
				
				if (SetEnergyFunctionCost(*fn))
				{
					log.LogDebug(" (Follow up Energy Function) \n\n");
					continue;
				} 
				else if (fn != nullptr)
				{
					//Normal function so traverse this function recursively
					//calc cost of tail call first
					// then traverse function
					log.LogDebug(" (Normal function needs traversing \n\n");
					IterateFunction(OrderedF[fn]);
					
				}
				else
				{
					log.LogDebug(" (normal instruction): ");
					SetInstructionCost(inst);
					
					//The last instruction of a basic Block is always of type br meaning it is a normal instruction
					//check if the current instruction is the last instruction of the basic block
					//also check if it is a loop
					if (BB->getTerminator() == &inst && IsLoopStart(*BB))
					{
						log.LogDebug("========================================LOOP START===============================\n");
						//When entered here the current Basic Block is the start of a loop
						LoopOrderedSet& currentLoop = GetLoop(*BB);
						currentLoop.currentTrip++;
						if (currentLoop.tripCount >= currentLoop.currentTrip)
						{
							log.LogDebug("COUNTER: " + std::to_string(currentLoop.currentTrip) + " / " + std::to_string(currentLoop.tripCount) + "\n");
							IterateFunction(currentLoop.loop);
						}
						else
						{
							currentLoop.currentTrip = 1;
							log.LogDebug("========================================LOOP END=================================\n");
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
		log.LogDebug("Function:  " + F.getName().str() + "\n");
		
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
			//	log.LogDebug("Not part of a loop: "+ getSimpleNodeLabel(SCCBBs[0]) + "\n");
			
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
				log.LogDebug("Loop from BB: " + getSimpleNodeLabel(Succ) + "  loop to BB: " + getSimpleNodeLabel(*BBI) + "\n");
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
	log.LogDebug(" LOOP: " + getSimpleNodeLabel(BBbegin) + "\n");
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

	BELOW THIS POINT ARE TEST FUNCTIONS. SHOULD BE REMOVED

=======================================================================================================================*/
/*
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

		log.LogDebug(" ");
		inst.dump();

		//Check if the instruction is a function call
		if (llvm::Function *tailCall = llvm::dyn_cast<llvm::Function>(callInst->getCalledValue()->stripPointerCasts()))
		{
			log.LogDebug("   ===> Call to Function => " + tailCall->getName().str());

			if (tailCall->getName() == LOOP_TRIPCOUNT)
			{
				llvm::ConstantInt* tripCountConstant = llvm::dyn_cast<llvm::ConstantInt>(callInst->getOperand(0));
				int64_t tripCount = tripCountConstant->getSExtValue();

				log.LogDebug("Basic block " + getSimpleNodeLabel(&BB) + " is part of a LOOP with trip count " + std::to_string(tripCount) + "\n");
				

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
					log.LogDebug("  => EName: " + eName.str() + " STOP powerdraw (" + found->second.str() + ")" + "\n");
				}
				else
				{
					//only add to vector when powerdraw is greater than 0. Else it is just time
					unsigned int pd = std::stoi(tailCall->getFnAttribute("pd").getValueAsString().str());
					if (pd > 0)
						m_eFunctions.insert(std::pair<llvm::StringRef, llvm::StringRef>(eName, tailCall->getFnAttribute("pd").getValueAsString()));


					log.LogDebug(" => EName: " + eName.str()
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

			log.LogDebug(L->getNumBackEdges() + "\n");

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
			
		log.LogDebug("  SCC: ");

		if (SCCBBs.size() == 1)
		{
			log.LogDebug(getSimpleNodeLabel(SCCBBs[0]) + " (no loop)\n");
			continue;
		}

			
		LoopOrderedSet los;

		for (std::vector<llvm::BasicBlock *>::const_iterator BBI = SCCBBs.begin(),
			BBIE = SCCBBs.end();
			BBI != BBIE; ++BBI) 
		{
			los.loop.push_back(*BBI);

			log.LogDebug(getSimpleNodeLabel(*BBI) + "  ");

			const llvm::BasicBlock* BB = *BBI;
			const llvm::TerminatorInst *TInst = BB->getTerminator();
			for (unsigned I = 0, NSucc = TInst->getNumSuccessors(); I < NSucc; ++I) {
				llvm::BasicBlock *Succ = TInst->getSuccessor(I);
				
				if (std::find(los.loop.begin(), los.loop.end(), Succ) != los.loop.end())
				{
					log.LogDebug("(nested loop to  " + getSimpleNodeLabel(Succ) + ") ");
				}
				
			}

		}
		log.LogDebug("\n");
		
		los.tripCount = los.loop.size();
		LoopSet[SCCBBs[0]] = los;


	}
}








//TODO: clean this up
void CallGraphVisitor::PostOrderTraversal(llvm::Module& M)
{

	for (llvm::Function& F : M)
	{
		log.LogDebug("Basic blocks of " + F.getName().str() + " in post-order:\n");
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
/*
}

void CallGraphVisitor::PrintAllLoops()
{
	
	for (llvm::DenseMap<llvm::BasicBlock*, LoopOrderedSet>::iterator IT = LoopSet.begin(),
		IE = LoopSet.end(); IT != IE; IT++)
	{
		log.LogDebug(getSimpleNodeLabel(IT->first) + ": ");
		LoopOrderedSet set = IT->second;

		log.LogDebug("(" + std::to_string(set.tripCount) + ", "+ std::to_string(set.loopSize) + "): ");

		for (llvm::BasicBlock* bb : set.loop)
		{
			log.LogDebug(getSimpleNodeLabel(bb));
		}
		
		log.LogDebug("\n");
	}

}

void CallGraphVisitor::Print(OrderedBB & F)
{
	for (llvm::BasicBlock* bb : F)
	{
		log.LogDebug(getSimpleNodeLabel(bb));
		auto x = LoopSet.find(bb);
		if (x != LoopSet.end())
		{
			//has a loop
			for (llvm::BasicBlock* b : LoopSet[bb].loop)
			{
			  log.LogDebug(getSimpleNodeLabel(b) + ", ");
			}

		}
			

		log.LogDebug("\n");
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
