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

	//Start at the entry point of main function or any other function that is the module entry
	// at this point we have path #0
	AddPath();
	TraveseFunction(pm);
	
	log.LogConsole("Ok\n");

	testPrint();
	return 0;
}

void EnergyCalculator::AddPath()
{

	ProgramPath p;
	p.ID = programPathID;
	ProgramPathCosts[programPathID] = p;

	if (programPathID > 0)
	{


	//	//this means the path was incremented by 1
		unsigned prevPath = programPathID - 1; // get the previous path
		SubPaths sp = ProgramPathCosts[prevPath].subPaths; // get all subpaths of previous ProgramPath
		//if (sp.size() > 0)
		//	sp.pop_back(); // remove the last entry from the subpath
		SubPaths prevSubPaths;

		for (SubPaths::iterator IT = sp.begin(), ITE = sp.end(); IT != ITE; ++IT)
		{
			if (IT->first < m_nestedLevel)
			{
				prevSubPaths[IT->first] = IT->second;
			}
		}

		ProgramPathCosts[programPathID].subPaths = prevSubPaths; // add the subpaths to the current programpath
	}
	
}

void EnergyCalculator::TraveseFunction(PathMap & pathMap)
{


	//log.SetLevel(Logging::DEBUG);
	//iterate all possible paths
	for (PathMap::iterator PI = pathMap.begin(), PIE = pathMap.end(); PI != PIE; ++PI)
	{

		//if the pathNo > 0  then we are changing paths
		// each function can have multiple paths but the first one is always 0
		// This means when the path is not 0 but for instance 1 we have new path
		// everytime we pass this code here below and the path > 0 we must add 1 to 
		// possible the final program path.
		if (PI->first > 0) {
			programPathID++;
			AddPath();
		}
			
		
		
		log.LogDebug("\n\n=====ProgramPathID: " + std::to_string(programPathID) + "=====\n\n");
		
		log.LogDebug("\n\n===========> SubPath " + std::to_string(PI->first) + ":\n\n");


		if (ProgramPathCosts[programPathID].subPaths[m_nestedLevel].size() == 0)
		{
			InstructionCostVec icv;
			ProgramPathCosts[programPathID].subPaths[m_nestedLevel] = icv;
		}


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
				log.LogDebug("\n========================================" + fn->getName().str() + "========================================\n");
				log.LogDebug(" (Normal function needs traversing \n\n");
				PathMap subPaths = pathAnalysis->GetFunctionPaths(*fn);
				m_nestedLevel++;
				TraveseFunction(subPaths);
				m_nestedLevel--;
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
					log.LogDebug("\n========================================" + fn->getName().str() + "========================================\n");
					log.LogDebug(" (Normal function needs traversing \n\n");
					m_nestedLevel++;
					TraveseFunction(pathAnalysis->GetFunctionPaths(*fn));
					m_nestedLevel--;
				}
				else
				{
					log.LogDebug(" (normal instruction): ");
					SetInstructionCost(*F, *BB, inst);
	
					
					if (BB->getTerminator() == &inst && loopAnalysis->IsLoopEnd(BB))
					{
						log.LogDebug("========================================LOOP START===============================\n");
						Edge* loopEdge = loopAnalysis->GetLoopEdge(BB);
						loopEdge->currentTripCount++;

						if (loopEdge->loopTripCount >= loopEdge->currentTripCount)
						{

							log.LogDebug("COUNTER: " + std::to_string(loopEdge->currentTripCount) + " / " + std::to_string(loopEdge->loopTripCount) + "\n");
							OrderedBBSet loopSet = loopAnalysis->GetLoopOrderedBBSet(*loopEdge, OBB);
							TraversePaths(loopSet);
						}
						else
						{
							loopEdge->currentTripCount = 1;
							log.LogDebug("========================================LOOP END=================================\n");
						}

					}
				}
			}

		}
	}


}


void EnergyCalculator::SetInstructionCost(llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I)
{
	
	InstructionCost IC = wcetAnalysis->GetInstructionCost(F, BB, I);
	InstructionCostVec icv;
	if (ProgramPathCosts[programPathID].subPaths[m_nestedLevel].size() > 0)
	{
		icv = ProgramPathCosts[programPathID].subPaths[m_nestedLevel];
	}

	icv.push_back(IC);
	ProgramPathCosts[programPathID].subPaths[m_nestedLevel] = icv;

	ProgramPathCosts[programPathID].totalWcet += IC.InstrCost;

	log.LogDebug(" cost: " + std::to_string(IC.InstrCost) + "\n");

}

bool  EnergyCalculator::SetEnergyFunctionCost(const llvm::Function& F)
{
	if (&F != nullptr && HasEnergyAnnotation(F))
	{
		//Set first run to false;
		if (m_firstRun)
			m_firstRun = false;

		if (F.hasFnAttribute(ENERGY_FUNCTION_NAME))
		{
			
			//TODO: Calculation of energy function powerdraw and single consumption
			EnergyFunctions ef = ProgramPathCosts[programPathID].energyFunctions;
			auto energyFunc = std::find(ef.begin(), ef.end(), &F);
			if (energyFunc != ef.end())
			{
				const llvm::Function* enf = *energyFunc;
				//already exists;
				//is it stacking or should it be removed?
				
			}
			else
			{
				//add it
				ProgramPathCosts[programPathID].energyFunctions.push_back(&F);
			}


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

		/*name = F.getFnAttribute(ENERGY_FUNCTION_NAME).getValueAsString();
			if (F.hasFnAttribute(ENERGY_TEMPORAL_CONSUMPTION))
			{
				pd = std::stoi(F.getFnAttribute(ENERGY_TEMPORAL_CONSUMPTION).getValueAsString().str());
			}*/
		/*if (F.hasFnAttribute(ENERGY_FUNCTION_NAME))
			pd = std::stoi(F.getFnAttribute(ENERGY_TEMPORAL_CONSUMPTION).getValueAsString().str());

		if (F.hasFnAttribute(ENERGY_CONSUMPTION))
			ec = std::stoi(F.getFnAttribute(ENERGY_CONSUMPTION).getValueAsString().str());

		if (F.hasFnAttribute(ENERGY_TIME_UNIT))
			t = std::stoi(F.getFnAttribute(ENERGY_TIME_UNIT).getValueAsString().str());

		log.LogDebug(F.getName().str() + " has Energy values: Time-dependent consumption=" + std::to_string(pd) + " One-time energy consumption=" + std::to_string(ec) + " Time=" + std::to_string(t) + "!\n");*/

		return true;
	}
	return false;
}


void EnergyCalculator::testPrint()
{
	for (PathCosts::iterator IT = ProgramPathCosts.begin(), ITE = ProgramPathCosts.end(); IT != ITE; ++IT)
	{
		ProgramPath pp = IT->second;
		log.LogConsole("===================MainProgram Path ID: " + std::to_string(pp.ID) + "=============\n");
		for (SubPaths::iterator SPIT = pp.subPaths.begin(), SPIET = pp.subPaths.end(); SPIT != SPIET; ++SPIT)
		{
			
			log.LogConsole("  Nested Level: " + std::to_string(SPIT->first) + "\n");
			/*InstructionCostVec ICV = SPIT->second;
			for (InstructionCost IC : ICV)
			{
				log.LogConsole("   " + getSimpleNodeLabel(IC.parentBB) + "\n");
			}
		*/	
		}

		log.LogConsole("  Total WCET: " + std::to_string(pp.totalWcet) + "\n\n");


	}

}