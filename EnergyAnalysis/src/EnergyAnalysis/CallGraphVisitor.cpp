#include "CallGraphVisitor.h"



void CallGraphVisitor::visit(EnergyModule& em)
{
	llvm::Module& module = em.GetLLVMModule();
	
	//llvm::CallGraph cfg = llvm::CallGraph(module);
	//cfg.dump();
	
	//PostOrderTraversal(module);
	llvm::Function *mainFunction = GetModuleEntryPoint(module);

	TraverseFunction(*mainFunction);
	
	OrderedBB bbs = OrderedF[mainFunction];
	for (llvm::BasicBlock* b : bbs)
	{
		std::cout << getSimpleNodeLabel(b) << std::endl;
		TraverseInstructions(*b);

	}
}


void CallGraphVisitor::IterateSCC(llvm::BasicBlock& BB)
{
	llvm::Function* parent = BB.getParent();

}


void CallGraphVisitor::TraverseInstructions(llvm::BasicBlock& BB)
{
	std::vector<llvm::StringRef> eFunctions; //temp store

	for (llvm::Instruction& inst : BB)
	{
		std::cout << " ";
		inst.dump();
		llvm::CallInst * callInst = llvm::dyn_cast<llvm::CallInst>(&inst);

		if (callInst == nullptr)
			continue;

		if (llvm::Function *voidFunc = llvm::dyn_cast<llvm::Function>(callInst->getCalledValue()->stripPointerCasts()))
		{
			std::cout << "   ===> Call to Function => " << voidFunc->getName().str();

			if (voidFunc->hasFnAttribute("Energy"))
			{
				llvm::StringRef eName = voidFunc->getFnAttribute("name").getValueAsString();

				if (std::binary_search(eFunctions.begin(), eFunctions.end(), eName))
				{
					std::cout << "  => EName: " << eName.str() << " STOP powerdraw" << std::endl;
				}
				else
				{
					eFunctions.push_back(eName);
					std::cout << " => EName: " << eName.str()
						<< " (pd="
						<< voidFunc->getFnAttribute("pd").getValueAsString().str() << " ec="
						<< voidFunc->getFnAttribute("ec").getValueAsString().str() << " t="
						<< voidFunc->getFnAttribute("t").getValueAsString().str() << ")"
						<< std::endl;
				}

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

	for (llvm::scc_iterator<llvm::Function *> I = llvm::scc_begin(&F),
		IE = llvm::scc_end(&F);
		I != IE; ++I)
	{
		OrderedBB &SCCBBs = const_cast<OrderedBB&>(*I); //cast to non const
		std::reverse(SCCBBs.begin(), SCCBBs.end()); //reverse the order
		//SCCs[&F] = SCCBBs;

		llvm::outs() << "  SCC: ";
		for (std::vector<llvm::BasicBlock *>::const_iterator BBI = SCCBBs.begin(),
			BBIE = SCCBBs.end();
			BBI != BBIE; ++BBI) {
			llvm::outs() << getSimpleNodeLabel(*BBI) << "  ";
		}
		llvm::outs() << "\n";
		
	}
	

}


// The main function is a C/C++ entry point. LLVM is not aware of the real module entry point.
//TODO: allow annotating the entry function 
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







CallGraphVisitor::CallGraphVisitor()
{
	



}


CallGraphVisitor::~CallGraphVisitor()
{
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
