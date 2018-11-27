#include "LoopAnalysisVisitor.h"

void LoopAnalysisVisitor::visit(EnergyModule & em)
{
	log.LogConsole("Analysing Loops...");
	llvm::Module& module = em.GetLLVMModule();

	llvm::legacy::FunctionPassManager pass(&module);
	LoopAnalysisPass* loopAnalysis = new LoopAnalysisPass();
	pass.add(loopAnalysis);

	for (llvm::Function& fn : module)
	{
		pass.run(fn);
	}

	loopEdges = loopAnalysis->GetLoopEdges();
	log.LogConsole("Ok\n");
}

void LoopAnalysisVisitor::Print()
{
	for (Edge& e : loopEdges)
	{
		log.LogConsole(" From " + getSimpleNodeLabel(e.from) + " => " + getSimpleNodeLabel(e.to) + ": aantal trips: " + std::to_string(e.loopTripCount) + "\n");
	}

}
