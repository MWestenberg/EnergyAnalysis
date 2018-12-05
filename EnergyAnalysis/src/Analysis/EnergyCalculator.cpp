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

	return 0;
}

void EnergyCalculator::AddPath()
{

	ProgramPath p;
	p.ID = programPathID;
	ProgramPathCosts[programPathID] = p;

	if (programPathID > 0)
	{
		// This means the path was incremented by 1
		// and we must copy the previous level to the current ProgramPath
	
		// get the previous path ID
		unsigned prevPath = programPathID - 1; 

		// total program wcet
		WCETperLevel wcetPrevLevel = ProgramPathCosts[prevPath].wcet; 

		// Total numbers of joules per level thus far
		JoulesPerlevel joulesPrevLevel = ProgramPathCosts[prevPath].joules; 

		// Loop until the current nested loop (not counting the current level
		// that is why it is < and not <=
		for (unsigned i = 0; i < m_nestedLevel; i++)
		{
			
			if (wcetPrevLevel.find(i) != wcetPrevLevel.end())
				ProgramPathCosts[programPathID].wcet[i] = wcetPrevLevel[i];

			if (joulesPrevLevel.find(i) != joulesPrevLevel.end())
				ProgramPathCosts[programPathID].joules[i] = joulesPrevLevel[i];

		}

	}
	
}

void EnergyCalculator::TraveseFunction(PathMap & pathMap)
{

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

		TraversePaths(PI->second);
	}

}

void EnergyCalculator::TraversePaths(OrderedBBSet& OBB)
{

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


			// if not the first run always set the instruction cost of the instruction in question
			if (fn == nullptr) {
				log.LogDebug(" (normal instruction)\n");
				SetInstructionCost(*F, *BB, inst);
			}

			// if fn is function we must traverse
			if (fn != nullptr)
			{
				//If the function is an energy function the energy attributes will be set
				if (!SetEnergyFunctionCost(*fn, *BB, inst))
				{
					// We found a non energy function and must traverse it.
					log.LogDebug("\n========================================" + fn->getName().str() + "========================================\n");
					log.LogDebug(" (Normal function needs traversing \n\n");
					// if this branch is reached the function is a real function
					PathMap subPaths = pathAnalysis->GetFunctionPaths(*fn);
					m_nestedLevel++;
					TraveseFunction(subPaths);
					m_nestedLevel--;
				}

			}
			// TODO: now we traverse the loop but we should traverse it once and store the total joules and wcet and multiply that to the 
			// remaining iterations and store it in the Edge itself.
			// Next time around we can simply take the wcet and number of joulse from it.
			// Check if the instruction is branch and a loop
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


void EnergyCalculator::SetInstructionCost(llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I)
{
	InstructionCost IC = wcetAnalysis->GetInstructionCost(F, BB, I);
	AddWCET(IC.InstrCost);
	log.LogDebug(" cost: " + std::to_string(IC.InstrCost) + "\n");
	AddPowerdraw(IC);

	

}

bool  EnergyCalculator::SetEnergyFunctionCost(const llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I)
{
	if (&F != nullptr && HasEnergyAnnotation(F))
	{
	
		EnergyValue ev = GetEnergyValue(F);

		log.LogDebug("  EnergyFunction Name = " + ev.name.str() + " pd = " + std::to_string(ev.pd) + " ec = " + std::to_string(ev.ec) + " wcet = " + std::to_string(ev.t)  + "\n");

		if (ev.HasValues())
		{
			// We consider Time t in this case equal to a cycle
			// Add the wcet of the energy function to the total WCET per Level
			AddWCET(ev.t);

			//Add the non temperal energy consumption to the number of joules per level
			AddJoules(ev.ec);

			InstructionCost IC = InstructionCost(&I, &BB, ev.t);
			
			// Add the Energy fucntion to the list of current functions of the current NestedLevel
				// and the current ProgramPath

			// This is a toggle
			// when the current energy function has a powerdraw the cost of the function itself
			// should not count as a powerdraw because that is captured by the ec value
			// When the powerdraw == 0 it is a stop function and must be added first 
			// before calcuting powerdraw to prevent the stop function itself to be counted 
			// as powerdraw too
			if (ev.pd > 0)
			{
				AddPowerdraw(IC);
				ProgramPathCosts[programPathID].energyConsumption[m_nestedLevel].push_back(ev);	
			}
			else
			{
				ProgramPathCosts[programPathID].energyConsumption[m_nestedLevel].push_back(ev);
				AddPowerdraw(IC);
			}
		}
		return true;
	}
	return false;
}


void EnergyCalculator::AddWCET(WCET executionTime)
{
	if (ProgramPathCosts[programPathID].wcet.size() > 0)
		ProgramPathCosts[programPathID].wcet[m_nestedLevel] += executionTime;
	else
		ProgramPathCosts[programPathID].wcet[m_nestedLevel] = executionTime;
}

void EnergyCalculator::AddJoules(Joules joules)
{
	if (ProgramPathCosts[programPathID].joules.size() > 0)
		ProgramPathCosts[programPathID].joules[m_nestedLevel] += joules;
	else
		ProgramPathCosts[programPathID].joules[m_nestedLevel] = joules;
}

// Energy Annotated functions are different
// At this point we should scan the available EnergyFunctions
// If for instance there is a function with a powerdraw and a name X
// we must iterate the EnergyFunctions and search for a function with the same name
// but has powerdraw set to 0. If this is the case we must not multiply the instruction
// cost with the powerdraw.
// If we cannot find we must multiply the current instruction Cost with the powerdraw
// and add the result to the nested level number of Joules of the current ProgramPath.
//
// Special case, when we start a powerdraw with a function and start it again before stopping
// should we calculate the powerdraw twice or not?
// I think not, because the name is the same. Stopping a powerdraw that was added twice
// could cause undefined behaviour. Will it stop the first, the second or both?
// I assume it would stop both. 
// The reason is because the energy function should activate an external component
// We assume that that component cannot be started twice. We assume it has a check that it is
// already started. If the external function would have a toggle mechanism
// this could be solved by adding the Energy function twice with the same name where the first would have 
// a powerdraw > 0 and the second would have a powedraw of 0 canceling it.

void EnergyCalculator::AddPowerdraw(const InstructionCost & IC)
{

	// Temporary vector
	EnergyValueVec evVector;

	// The energyconsuming function that have been added in this programpath.
	EnergyConsumptionPerLevel energyPerLevel = ProgramPathCosts[programPathID].energyConsumption;

	//here we must traverse all nestedLevels including the current one
	for (unsigned i = 0; i <= m_nestedLevel; i++)
	{
		if (energyPerLevel.find(i) != energyPerLevel.end())
		{
			for (EnergyValue ev : energyPerLevel[i])
			{
				auto found = std::find_if(evVector.begin(), evVector.end(), [&ev](const EnergyValue& e) { return e.name == ev.name;  });
				if (found != evVector.end())
				{
					EnergyValue evFound = *found;
					
					//case ID:	ev.pd	|	found.pd
					//	1		0		|	0			we have a powerdraw stop but found a stopper too, do nothing
					//	2		0		|	1			we have a powerdraw stop and found a powerdraw starter: Stop de powerdraw
					//  3		1		|	0			we have a started and found a stopper, it is restarted and will be counted
					//	4		1		|	1			double starter, this should not happen and is ignored.
					if (ev.pd > 0 && evFound.pd == 0) //case 3
						evVector.push_back(ev);
					else if (ev.pd == 0 && evFound.pd > 0)  //case 2
						evVector.erase(found);
					
				}
				else if (ev.pd > 0) //new powerdraw not found so add
					evVector.push_back(ev);
			}
		}

	}

	for (EnergyValue ev : evVector)
	{
		unsigned int powerdraw = ev.pd * IC.InstrCost;
		log.LogDebug(" ==> Powerdraw: " + ev.name.str() + ": " + std::to_string(ev.pd) + "  * " + std::to_string(IC.InstrCost) + " = " + std::to_string(powerdraw)  + "\n");
		ProgramPathCosts[programPathID].joules[m_nestedLevel] += powerdraw;
	}

}



void EnergyCalculator::Print()
{

	//iterate each ProgramPath
	for (PathCosts::iterator IT = ProgramPathCosts.begin(), ITE = ProgramPathCosts.end(); IT != ITE; ++IT)
	{
		ProgramPath pp = IT->second;
		log.LogConsole("===================MainProgram Path ID: " + std::to_string(pp.ID) + "=============\n");


		//iterate per level the WCET
		unsigned wcet = 0;
		for (WCETperLevel::iterator WIT = pp.wcet.begin(), WIET = pp.wcet.end(); WIT != WIET; ++WIT)
		{
			log.LogConsole("  Nested Level: " + std::to_string(WIT->first) + "\n");
			wcet += WIT->second;
		}

		// iterate per level the Joules
		unsigned joules = 0;
		for (WCETperLevel::iterator JIT = pp.joules.begin(), JIET = pp.joules.end(); JIT != JIET; ++JIT)
		{
			joules += JIT->second;
		}

		log.LogConsole("  Total WCET: " + std::to_string(wcet) + "\n\n");
		//log.LogConsole("  Total PowerdrawMP: " + std::to_string(pdwcet) + "\n\n");
		log.LogConsole("  Total Joules: ");
		log.RecursiveCommas(std::cout, joules);
		log.LogConsole("\n\n");


	}

}