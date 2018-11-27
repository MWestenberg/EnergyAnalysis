#include "WCETAnalysisVisitor.h"

void WCETAnalysisVisitor::visit(EnergyModule & em)
{
	log.LogConsole("Calculating Costs...");
	
	llvm::Module& module = em.GetLLVMModule();
	
	llvm::legacy::FunctionPassManager pass(&module);
	WCETAnalysisPass* wcetAnalysis = new WCETAnalysisPass();
	pass.add(wcetAnalysis);
	for (llvm::Function& fn : module)
	{
		pass.run(fn);
		InstructionCostVec ICV;

		unsigned fnCost = 0;

		for (llvm::BasicBlock &B : fn) 
		{
			unsigned bbCost = 0;
			for (llvm::Instruction &Inst : B)
			{
				unsigned cost = wcetAnalysis->getInstructionCost(&Inst);
				if (cost == ULONG_MAX)
					cost = 1;
				bbCost += cost;
				InstructionCost IC(&Inst, &B, cost);
				IC.AddCumulativeCost(bbCost);
				ICV.push_back(IC);
			}
			fnCost += bbCost;
			FCM[&fn][&B] = ICV;
		}

		
	}
	
	log.LogConsole("Ok\n");
}

void WCETAnalysisVisitor::Print()
{
	for (FunctionCostMap::iterator I = FCM.begin(), IE = FCM.end(); I != IE; ++I)
	{
		llvm::Function& F = *I->first;
		BBCostMap& bbCostMap = I->second;
		
		log.LogConsole("Function: " + F.getName().str() + "\n");
		unsigned totalFnCost = 0;
		for (BBCostMap::iterator BI = bbCostMap.begin(), BIE = bbCostMap.end(); BI != BIE; ++BI)
		{
			llvm::BasicBlock& BB = *BI->first;
			InstructionCostVec ICV = BI->second;

			unsigned totalBBCost = 0;

			log.LogConsole(" BB: " + getSimpleNodeLabel(&BB) + "\n");
			for (InstructionCost IC : ICV)
			{
				totalBBCost += IC.InstrCost;
				llvm::Instruction* instr = IC.instruction;
				std::string fnName = "";
				if (instr->getOpcode() == llvm::Instruction::Call)
				{
					if (llvm::Function* call = IsFunction(*instr)) {
						fnName = call->getName().str();
						if (HasEnergyAnnotation(*call))
						{
							fnName += " (Energy Function)";
						}
						if (HasFunctionName(*call, LOOP_TRIPCOUNT))
						{
							fnName += " (Loop Annotation)";
						}
					}
						
				}
				log.LogConsole("   Instr: " + std::string(instr->getOpcodeName()) + " " + fnName + "\n");
				log.LogConsole("   Cost: " + std::to_string(IC.InstrCost) + "\n");
				log.LogConsole("   Cumulative Cost: " + std::to_string(totalBBCost) + "\n");
				
			}
			totalFnCost += totalBBCost;
			log.LogConsole(" Total BB Cost: " + std::to_string(totalBBCost) + "\n\n");
		}
		log.LogConsole(" Total Function Cost: " + std::to_string(totalFnCost) + "\n\n");
		
		
	}


}
