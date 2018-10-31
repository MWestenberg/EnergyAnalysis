#include "CallGraphVisitor.h"



void CallGraphVisitor::visit(EnergyModule& em)
{
	llvm::Module& module = em.GetLLVMModule();
	
	for (llvm::Function& F : module)
	{
		
		if (F.getName().str() != "main")
			continue;

		for (llvm::BasicBlock& BB : F)
		{
			for (llvm::Instruction& I : BB)
			{
				llvm::CallInst * callInst = llvm::dyn_cast<llvm::CallInst>(&I);
				

				if (callInst == nullptr)
					continue;

				if (llvm::Function *calledFunction = callInst->getCalledFunction()) {
					//std::cout << calledFunction->getName().str() << std::endl;
				}
			}
		}
	}
	
	llvm::CallGraph cfg = llvm::CallGraph(module);
	//cfg.dump();

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



}


CallGraphVisitor::CallGraphVisitor()
{
	



}


CallGraphVisitor::~CallGraphVisitor()
{
}
