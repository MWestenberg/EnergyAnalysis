#include "EnergyCalculator.h"

int EnergyCalculator::visit(EnergyModule & em)
{
	log.LogConsole("Calculating Energy Consumption...");
	module = &em.GetLLVMModule();
	
	//MainFunction is the entry function defined in Constants.
	llvm::Function* mainFunction = GetModuleEntryPoint(*module);
	if (mainFunction == nullptr)
		return EnergyAnalysis::E_MESSAGE_INVALID_ENTRY_POINT;

	
	//All possible paths in the main Function
	PathMap& pm = pathAnalysis->GetFunctionPaths(*mainFunction);
	TraveseFunction(pm);
	
	log.LogConsole("Ok\n");
	return 0;
}

void EnergyCalculator::TraveseFunction(PathMap & pathMap)
{
	log.SetLevel(Logging::DEBUG);
	//iterate all possible paths
	for (PathMap::iterator PI = pathMap.begin(), PIE = pathMap.end(); PI != PIE; ++PI)
	{
		log.LogDebug("\n\n===========> Path " + std::to_string(PI->first) + ":\n\n");
		//for each possible path traverse it's set of Basic blocks
		TraversePaths(PI->second);
	}

}

void EnergyCalculator::TraversePaths(OrderedBBSet& OBB)
{
	
	//pathAnalysis->printPath(OBB);
	for (llvm::BasicBlock* BB : OBB)
	{
		llvm::Function* F = BB->getParent();

		log.LogDebug("========================================" + getSimpleNodeLabel(BB) + "========================================\n");
		for (llvm::Instruction& inst : *BB)
		{
			if (log.GetLevel() == log.DEBUG)
				inst.dump();

			//retrieve call instruction
			llvm::Function* fn = IsFunction(inst);

			//First run means we can ignore the non energy functions
			if (m_firstRun && fn == nullptr) {
				log.LogDebug(" (first run: ignored) \n\n");
				continue; //no need to calc instruction cost
			}


			if (m_firstRun && fn != nullptr)
			{

				//if (HasEnergyAnnotation(*fn))
				if (SetEnergyFunctionCost(*fn))
				{
					log.LogDebug(" (First Energy Function) \n\n");
					continue;
				}

				//Normal function so traverse this function recursively
				//calc cost of tail call first
				// then traverse function
				log.LogDebug(" (Normal function needs traversing \n\n");
				TraveseFunction(pathAnalysis->GetFunctionPaths(*fn));
			}
			else
			{
				//In case of first run, continue until a function is found
				if (m_firstRun) {
					log.LogDebug(" (first run: ignored) \n\n");
					continue;
				}

				//if (HasEnergyAnnotation(*fn))
				if (SetEnergyFunctionCost(*fn))
				{
					log.LogDebug(" (Follow up Energy Function) \n\n");
					continue;
				}
				else if (fn != nullptr)
				{
					//Normal function so traverse this function recursively
					//calc cost of tail call first
					// then traverse function
					log.LogDebug(" (Normal function needs traversing \n\n");
					TraveseFunction(pathAnalysis->GetFunctionPaths(*fn));

				}
				else
				{
					log.LogDebug(" (normal instruction): ");
					SetInstructionCost(*F, *BB, inst);
	
					//The last instruction of a basic Block is always of type br meaning it is a normal instruction
					//check if the current instruction is the last instruction of the basic block
					//also check if it is a loop
					
					if (BB->getTerminator() == &inst && loopAnalysis->IsLoopStart(BB))
					{
						log.LogDebug("========================================LOOP START===============================\n");
					//	//When entered here the current Basic Block is the start of a loop
					//	LoopOrderedSet& currentLoop = GetLoop(*BB);
					//	currentLoop.currentTrip++;
					//	if (currentLoop.tripCount >= currentLoop.currentTrip)
					//	{
					//		log.LogDebug("COUNTER: " + std::to_string(currentLoop.currentTrip) + " / " + std::to_string(currentLoop.tripCount) + "\n");
					//		IterateFunction(currentLoop.loop);
					//	}
					//	else
					//	{
					//		currentLoop.currentTrip = 1;
					//		log.LogDebug("========================================LOOP END=================================\n");
					//	}

					}
				}
			}

		}
	}


}


void EnergyCalculator::SetInstructionCost(llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I)
{
	
	InstructionCost IC = wcetAnalysis->GetInstructionCost(F, BB, I);
	log.LogDebug(" cost: " + std::to_string(IC.InstrCost) + "\n");

}

bool  EnergyCalculator::SetEnergyFunctionCost(llvm::Function& F)
{
	if (&F != nullptr && HasEnergyAnnotation(F))
	{
		//Set first run to false;
		if (m_firstRun)
			m_firstRun = false;

		llvm::StringRef name = "";
		signed int pd = 0;
		unsigned int ec = 0;
		unsigned int t = 0;

		if (F.hasFnAttribute(ENERGY_FUNCTION_NAME))
		{
			name = F.getFnAttribute(ENERGY_FUNCTION_NAME).getValueAsString();
			if (F.hasFnAttribute(ENERGY_TEMPORAL_CONSUMPTION))
			{
				pd = std::stoi(F.getFnAttribute(ENERGY_TEMPORAL_CONSUMPTION).getValueAsString().str());
			}

			//TODO: Calculation of energy function powerdraw and single consumption

			//// below is not ready please finish.
			//if (IsFirstEncounter(F)) {
			//	EnergyFunction ef;
			//	ef[&F] = pd;
			//	// add the powerdraw to Joules counter and to the powerdraw counter
			//}
			//else
			//{


			//}

		}


		if (F.hasFnAttribute(ENERGY_FUNCTION_NAME))
			pd = std::stoi(F.getFnAttribute(ENERGY_TEMPORAL_CONSUMPTION).getValueAsString().str());

		if (F.hasFnAttribute(ENERGY_CONSUMPTION))
			ec = std::stoi(F.getFnAttribute(ENERGY_CONSUMPTION).getValueAsString().str());

		if (F.hasFnAttribute(ENERGY_TIME_UNIT))
			t = std::stoi(F.getFnAttribute(ENERGY_TIME_UNIT).getValueAsString().str());

		log.LogInfo(F.getName().str() + " has Energy values: Time-dependent consumption=" + std::to_string(pd) + " One-time energy consumption=" + std::to_string(ec) + " Time=" + std::to_string(t) + "!\n");

		return true;
	}
	return false;
}