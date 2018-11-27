#include "WCETCostModelAnalysisPass.h"





char WCETCostModelAnalysisPass::ID = 0;

unsigned WCETCostModelAnalysisPass::getInstructionCost(const llvm::Instruction * I) const
{
	int cost  = TTI->getInstructionCost(I, llvm::TargetTransformInfo::TCK_Latency);
	if (cost != (unsigned)-1) {
		
		if (cost > 1)
			llvm::errs() << "JOEPIE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << "\n";
		return cost;

	}
		
	return 0;
}

bool WCETCostModelAnalysisPass::runOnFunction(llvm::Function & F)
{
	//this->F = &F;
	//llvm::PMDataManager *DM = getAsPMDataManager();
	//llvm::AnalysisResolver *AR = new llvm::AnalysisResolver(*DM);
	//setResolver(AR);
	//setTopLevelManager(new WCETCostModelAnalysisPass());

	//recordAvailableAnalysis(new llvm::TargetTransformInfoWrapperPass());
	//auto *TTIWP = getAnalysisIfAvailable<llvm::TargetTransformInfoWrapperPass>();
	//TTI = TTIWP ? &TTIWP->getTTI(F) : nullptr;



	return false;
}

void WCETCostModelAnalysisPass::print(llvm::raw_ostream & OS, const llvm::Module *) const
{
	if (!F)
		return;

	for (llvm::BasicBlock &B : *F) {
		for (llvm::Instruction &Inst : B) {
			unsigned Cost = TTI->getInstructionCost(&Inst, llvm::TargetTransformInfo::TCK_RecipThroughput);
			if (Cost != (unsigned)-1)
				OS << "Cost Model: Found an estimated cost of " << Cost;
			else
				OS << "Cost Model: Unknown cost";

			OS << " for instruction: " << Inst << "\n";
		}
	}
}

void WCETCostModelAnalysisPass::getAnalysisUsage(llvm::AnalysisUsage & AU) const
{
	AU.addRequired<llvm::DominatorTreeWrapperPass>();
	AU.addRequired<llvm::LoopInfoWrapperPass>();
	AU.addRequired<llvm::ScalarEvolutionWrapperPass>();
	AU.addRequired<llvm::TargetTransformInfoWrapperPass>();
	AU.setPreservesAll();
}
