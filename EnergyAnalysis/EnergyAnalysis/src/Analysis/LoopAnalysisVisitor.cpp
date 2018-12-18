#include "LoopAnalysisVisitor.h"

int LoopAnalysisVisitor::visit(EnergyModule & em)
{
	log.LogConsole("Analysing Loops...");
	llvm::Module& module = em.GetLLVMModule();

	llvm::legacy::FunctionPassManager pass(&module);

	// LoopAnalysis is a heap object and will be destroyed by the LLVM framework
	// at the end of this function. 
	LoopAnalysisPass* loopAnalysis = new LoopAnalysisPass();
	pass.add(loopAnalysis);

	for (llvm::Function& fn : module)
	{
		if (IsNotTraversable(fn))
			continue;
		
		if (!pass.run(fn))
			return loopAnalysis->GetPassResult();
	}

	EdgeCollection ec = loopAnalysis->GetLoopEdges();

	for (Edge* e : ec.GetAllLoopEdges())
	{
		loopEdges.AddEdge(*e);
	}
	


	log.LogConsole("Ok\n");
	return 0;
}

void LoopAnalysisVisitor::Print()
{
	log.LogConsole(PRINT_BEGIN);
	for (Edge* e : loopEdges.GetAllLoopEdges())
	{
		if (e->isSubLoop)
			log.LogConsole(" Subloop: ");
		log.LogConsole(" From " + getSimpleNodeLabel(e->from) + " => " + getSimpleNodeLabel(e->to) + ": aantal trips: " + std::to_string(e->loopTripCount) + "\n\n");
	}
	log.LogConsole(PRINT_END);

}
