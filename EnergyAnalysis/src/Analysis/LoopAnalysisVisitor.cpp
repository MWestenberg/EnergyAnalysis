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
		if (pass.run(fn))
			return loopAnalysis->GetPassResult();
	}

	loopEdges = loopAnalysis->GetLoopEdges();
	log.LogConsole("Ok\n");
	return 0;
}

void LoopAnalysisVisitor::Print()
{
	for (Edge& e : loopEdges)
	{
		log.LogConsole(" From " + getSimpleNodeLabel(e.from) + " => " + getSimpleNodeLabel(e.to) + ": aantal trips: " + std::to_string(e.loopTripCount) + "\n");
	}

}
