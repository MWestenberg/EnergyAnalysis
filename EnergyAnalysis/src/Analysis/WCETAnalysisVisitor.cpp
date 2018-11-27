#include "WCETAnalysisVisitor.h"

void WCETAnalysisVisitor::visit(EnergyModule & em)
{
	log.LogConsole("Calculating Costs...");
	module = &em.GetLLVMModule();
	pass = new llvm::legacy::FunctionPassManager(module);
	log.LogConsole("Ok\n");
}

void WCETAnalysisVisitor::CalculateCost(llvm::Function& F)
{

	WCETAnalysisPass* wcetAnalysis = new WCETAnalysisPass();
	pass->add(wcetAnalysis);
	pass->run(F);
	wcetAnalysis->print(llvm::outs(), module);

}
