#include "WCETCostModelAnalysis.h"


static llvm::cl::opt<llvm::TargetTransformInfo::TargetCostKind> wcetCostKind(
	"cost-kind-wcet", llvm::cl::desc("WCET Target cost kind"),
	llvm::cl::init(llvm::TargetTransformInfo::TCK_RecipThroughput),
	llvm::cl::values(clEnumValN(llvm::TargetTransformInfo::TCK_RecipThroughput,
		"throughput", "Reciprocal throughput"),
		clEnumValN(llvm::TargetTransformInfo::TCK_Latency,
			"latency", "Instruction latency"),
		clEnumValN(llvm::TargetTransformInfo::TCK_CodeSize,
			"code-size", "Code size"))
);

char WCETCostModelAnalysis::ID = 0;

unsigned WCETCostModelAnalysis::getInstructionCost(const llvm::Instruction * I) const
{
	return TTI->getInstructionCost(I, llvm::TargetTransformInfo::TCK_Latency);
	//if (cost != (unsigned)-1)
	//	return cost;
	//return 0;
}

bool WCETCostModelAnalysis::runOnFunction(llvm::Function & F)
{
	this->F = &F;
	llvm::PMDataManager *DM = getAsPMDataManager();
	llvm::AnalysisResolver *AR = new llvm::AnalysisResolver(*DM);
	setResolver(AR);
	setTopLevelManager(new WCETCostModelAnalysis());

	recordAvailableAnalysis(new llvm::TargetTransformInfoWrapperPass());
	auto *TTIWP = getAnalysisIfAvailable<llvm::TargetTransformInfoWrapperPass>();
	TTI = TTIWP ? &TTIWP->getTTI(F) : nullptr;

	return false;
}

void WCETCostModelAnalysis::print(llvm::raw_ostream & OS, const llvm::Module *) const
{
	if (!F)
		return;

	for (llvm::BasicBlock &B : *F) {
		for (llvm::Instruction &Inst : B) {
			unsigned Cost = TTI->getInstructionCost(&Inst, wcetCostKind);
			if (Cost != (unsigned)-1)
				OS << "Cost Model: Found an estimated cost of " << Cost;
			else
				OS << "Cost Model: Unknown cost";

			OS << " for instruction: " << Inst << "\n";
		}
	}
}

void WCETCostModelAnalysis::getAnalysisUsage(llvm::AnalysisUsage & AU) const
{
	AU.setPreservesAll();
}
