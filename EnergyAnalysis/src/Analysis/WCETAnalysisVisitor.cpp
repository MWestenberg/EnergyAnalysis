#include "WCETAnalysisVisitor.h"

int WCETAnalysisVisitor::visit(EnergyModule & em)
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

				if (IsFunction(Inst))
					continue;
				
				unsigned cost = wcetAnalysis->getInstructionCost(&Inst);
				if (cost >= INT_MAX)
					cost = 1;
				bbCost += cost;
				InstructionCost IC(&Inst, &B, cost);
				IC.AddCumulativeCost(bbCost);
				ICV.push_back(IC);
			}
			fnCost += bbCost;
			SetFunctionCost(fn, B, ICV);
		}

		
	}
	
	log.LogConsole("Ok\n");
	return 0;
}

void WCETAnalysisVisitor::Print()
{

	log.LogConsole(PRINT_BEGIN);
	for (FunctionCostMap::iterator I = FCM->begin(), IE = FCM->end(); I != IE; ++I)
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
					if (llvm::Function* call = IsFunction(*instr))
						fnName = " (function call: " +  call->getName().str() + ")";
						
				}
				log.LogConsole("   Instr: " + std::string(instr->getOpcodeName()) +  fnName + "\n");
				log.LogConsole("   Cost: " + std::to_string(IC.InstrCost) + "\n");
				log.LogConsole("   Cumulative Cost: " + std::to_string(totalBBCost) + "\n");
				
			}
			totalFnCost += totalBBCost;
			log.LogConsole(" Total BB Cost: " + std::to_string(totalBBCost) + "\n\n");
		}
		log.LogConsole(" Total Function Cost: " + std::to_string(totalFnCost) + "\n\n");
		
		
	}
	log.LogConsole(PRINT_END);


}

void WCETAnalysisVisitor::SetFunctionCost(llvm::Function& F, llvm::BasicBlock & B, InstructionCostVec ICV)
{
	FunctionCostMap& FCMRef = *FCM;
	FCMRef[&F][&B] = ICV;
}
