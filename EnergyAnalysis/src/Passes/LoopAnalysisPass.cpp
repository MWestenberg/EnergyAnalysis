#include "LoopAnalysisPass.h"

char LoopAnalysisPass::ID = 0;

void LoopAnalysisPass::getAnalysisUsage(llvm::AnalysisUsage & AU) const
{
	AU.addRequired<llvm::DominatorTreeWrapperPass>();
	AU.addRequired<llvm::LoopInfoWrapperPass>();
	AU.addRequired<llvm::ScalarEvolutionWrapperPass>();
	AU.setPreservesAll();
}

bool LoopAnalysisPass::runOnFunction(llvm::Function & F)
{
	log.LogDebug("================== Analysis of Function: " + F.getName().str() + "===================\n");

	LI = &getAnalysis<llvm::LoopInfoWrapperPass>().getLoopInfo();
	
	//LI.analyze(getAnalysis<llvm::DominatorTreeWrapperPass>().getDomTree());
	SE = &getAnalysis<llvm::ScalarEvolutionWrapperPass>().getSE();
		
		
	for (auto i = LI->begin(); i != LI->end(); ++i)
	{
		llvm::Loop *loop = *i;

		log.LogDebug("Found loops in function: " + F.getName().str() + "\n");
		unsigned int tripCount = FindNestedLoops(*loop);
		if (tripCount == 0)
		{
			log.LogError("Program halted");
			m_result = EnergyAnalysis::E_MESSAGE_UNDEFINED_LOOP;
			return false;
		}
	}
	return true;
}

// This function finds nested loops in a function. 
unsigned LoopAnalysisPass::FindNestedLoops(llvm::Loop & L)
{
	// Estimate the tripcount
	unsigned int tripCount = SE->getSmallConstantMaxTripCount(&L);
	
	tripCount = GetAnnotatedLoopTripCount(L);
	// Can we estimate the number of iterations on this loop?
	
	Edge edge(L.getHeader(), L.getLoopLatch());
	edge.isLoop = true;
	edge.loopTripCount = tripCount;
	loopEdges.push_back(edge);

	log.LogDebug("   Estimated Tripcount: " + std::to_string(edge.loopTripCount) + "\n");
	log.LogDebug("      From: " + getSimpleNodeLabel(L.getHeader()) + " => " + getSimpleNodeLabel(L.getLoopLatch()) + "\n");

	for (llvm::Loop* subLoop : L.getSubLoops())
	{
		log.LogDebug("    Subloop:\n");
		FindNestedLoops(*subLoop);
	}

	return tripCount;
}

unsigned LoopAnalysisPass::GetAnnotatedLoopTripCount(llvm::Loop & L)
{
	unsigned int tripCount = 0;
	for (llvm::BasicBlock* BB : L.getBlocks())
	{
		for (llvm::Instruction& inst : *BB)
		{
			llvm::Function* fn = IsFunction(inst);
			if (fn != nullptr && HasFunctionName(*fn, LOOP_TRIPCOUNT))
			{
				return GetLoopTripCount(inst);
			}
		}
	}

	return tripCount;
}


int LoopAnalysisPass::GetLoopTripCount(llvm::Instruction& I) const
{
	llvm::Function* fn = IsFunction(I);
	assert((fn != nullptr && HasFunctionName(*fn, LOOP_TRIPCOUNT)) && "The passed instruction must a function and have a name defined as LOOP_TRIPCOUNT and a integer as first parameter");

	llvm::CallInst * callInst = llvm::dyn_cast<llvm::CallInst>(&I);
	llvm::ConstantInt* tripCountConstant = llvm::dyn_cast<llvm::ConstantInt>(callInst->getOperand(0));
	int64_t tripCount = tripCountConstant->getSExtValue();
	return tripCount;
}