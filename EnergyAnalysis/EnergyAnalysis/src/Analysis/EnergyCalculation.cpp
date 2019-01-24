#include "EnergyCalculation.h"

int EnergyCalculation::visit(EnergyModule & em)
{

	
	module = &em.GetLLVMModule();
	if (!CreateBBTrace())
		return EnergyAnalysis::E_MESSAGE_ERROR_PARSE_TRACEFILE;

	log.LogConsole("\nTotal number of Basic Blocks: " + std::to_string(m_totalBBs) + "\n");
	log.LogConsole("Calculating energy consumption:\n");

	auto it = m_pathMap.begin();
	OrderedBBSet bbSet = it->second;

	if (!TraverseTrace(bbSet))
		return EnergyAnalysis::E_MESSAGE_ERROR_PARSE_TRACEFILE;

	//log.LogConsole("Ok\n");
	return 0;
}

void EnergyCalculation::PrintTrace()
{
	log.LogConsole("Input trace: \n");
	for (TraceMap::iterator it = m_traceMap.begin(), ie = m_traceMap.end(); it != ie; ++it)
	{
		log.LogConsole(" "+ it->first.str() + ":\n");
		for (llvm::StringRef& bb : it->second)
			log.LogConsole("  " + bb.str());
		log.LogConsole("\n\n");
	}
}

bool EnergyCalculation::CreateBBTrace()
{

	log.LogConsole("Creating trace...");

	//iterate all functions in the module
	for (llvm::Function& F : *module)
	{
		llvm::StringRef fnName = F.getName(); //get the name of the function from the llvm code
		if (m_traceMap.find(fnName) != m_traceMap.end()) // check if function name exists in tracemap
		{
			if (m_pathMap.find(&F) == m_pathMap.end())
				m_pathMap[&F]; // check if function is already added to work map

			// get all basic blocks names/ids and insert into vector
			std::vector<llvm::StringRef> nodeIDs = m_traceMap[fnName];
			for (llvm::StringRef& trID : nodeIDs)
			{ //loop all BB's

				for (llvm::BasicBlock& BB : F)
				{   
					//locate the BB in the Function
					std::string bbID = getSimpleNodeLabel(&BB);
					if (bbID.empty())
						continue; // not found continue
					if (bbID == trID.str()) {
						m_totalBBs++;
						m_pathMap[&F].push_back(&BB);
						break;
					}
						
				}
				
			}

			//if the following condition is true the count of both maps are not equal
			// thus something must have gone wrong.
			if (m_pathMap[&F].size() != m_traceMap[fnName].size()) {
				log.LogConsole("FAILURE ");
				return false;
			}
			
		}
		
	}
	
	pBar.SetTotalNumber(m_totalBBs);
	log.LogConsole("Ok\n");
	return true;
}

bool EnergyCalculation::TraverseTrace(OrderedBBSet& OBB)
{

	for (llvm::BasicBlock* BB : OBB)
	{
		
		pBar.update(1);
		std::string bbName = getSimpleNodeLabel(BB);
		
		//optimization: when the basic block was just processed it will have the exact same values
		// in terms of consumption and be added to the total.
		if (bbName == TECLastBB.name && !TECLastBB.name.empty())
		{
			//This means we already did this one
			TEC.Add(TECLastBB);
			continue;
		}
		else
		{
			TECLastBB.Reset();
			TECLastBB.name = bbName;
		}


		
		log.LogDebug("========================================" + getSimpleNodeLabel(BB) + "========================================\n");

		for (llvm::Instruction& inst : *BB)
		{
			if (log.GetLevel() == log.DEBUG)
				inst.dump();

			//retrieve call instruction
			llvm::Function* fn = IsFunction(inst);

			// if not the first run always set the instruction cost of the instruction in question
			if (fn == nullptr && !m_firstRun) {
				log.LogDebug(" (normal instruction)\n");
				//inst.dump();
				
				SetInstructionCost(*(BB->getParent()), *BB, inst);
				continue;
			}

			//// if fn is function we must traverse
			if (fn != nullptr)
			{
				//If the function is an energy function the energy attributes will be set
				if (!SetEnergyFunctionCost(*fn, *BB, inst))
				{
					// We found a non energy function and must traverse it.
					log.LogDebug("\n========================================" + fn->getName().str() + "========================================\n");
					log.LogDebug(" (Normal function needs traversing \n\n");

					// if this branch is reached the function is a real function
					if (m_pathMap.find(fn) == m_pathMap.end())
						return false; // the function is not in the trace. Something is wrong

					OrderedBBSet bbSet = m_pathMap[fn];
					
					if (!TraverseTrace(bbSet))
						return false; // something went wrong in the recursion
				}

			}
			

		}
		//double p = (m_currentBB / m_totalBBs) * 100;
		pBar.print();
		
		
	}
	return true;
}

//Sets the instruction cost
void EnergyCalculation::SetInstructionCost(llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I)
{
	InstructionCost IC = wcetAnalysis->GetInstructionCost(F, BB, I);
	AddCycles(IC.InstrCycle);
	log.LogDebug(" cost: " + std::to_string(IC.InstrCost) + "\n");
	AddPowerdraw(IC);
}

//Adds number of cycles to both the total as the last BB consumption
void EnergyCalculation::AddCycles(unsigned int cycles)
{
	TECLastBB.energyConsumption.numberOfCycles += cycles;
	TEC.energyConsumption.numberOfCycles += cycles;
}

//Adds number of jou;es to both the total as the last BB consumption
void EnergyCalculation::AddJoules(long double joules)
{
	TECLastBB.energyConsumption.Joules += joules;
	TEC.energyConsumption.Joules += joules;
}

//Adds number of execution time to both the total as the last BB consumption
void EnergyCalculation::AddExecTime(long double time)
{
	TECLastBB.energyConsumption.executionTime += time;
	TEC.energyConsumption.executionTime += time;
}

// Add an external component to memory for printing purposes
void EnergyCalculation::AddExternalComponent(ExternalComponent c)
{
	if (c.pd == 0 && c.ec == 0)
		return;

	auto found = std::find_if(TEC.externalComponents.begin(), TEC.externalComponents.end(), [&c](const ExternalComponent& e) { return e.name == c.name;  });
	if (found == TEC.externalComponents.end())
	{
		
		TEC.externalComponents.push_back(c);
	}
}

//Checks all previous components and trace. It calculates the powerdraw based on energy functions that
// are open / drawing power.
void EnergyCalculation::AddPowerdraw(const InstructionCost & IC)
{
	// Temporary vector that will only contain all energy functions / external components
	// that are draining power at the current point in time
	ExternalComponents externalComponents;

	//
	for (ExternalComponent extComp : TEC.externalComponentsTrace)
	{
		auto found = std::find_if(externalComponents.begin(), externalComponents.end(), [&extComp](const ExternalComponent& e) { return e.name == extComp.name;  });
		if (found != externalComponents.end())
		{
			ExternalComponent evFound = *found;
			
			//case ID:	ev.pd	|	found.pd
			//	1		0		|	0			we have a powerdraw stop but found a stopper too, do nothing
			//	2		0		|	1			we have a powerdraw stop and found a powerdraw starter: Stop de powerdraw
			//  3		1		|	0			we have a started and found a stopper, it is restarted and will be counted
			//	4		1		|	1			double starter, this should not happen and is ignored.
			if (extComp.pd > 0 && evFound.pd == 0) //case 3
				externalComponents.push_back(extComp);
			else if (extComp.pd == 0 && evFound.pd > 0)  //case 2
				externalComponents.erase(found);

		}
		else if (extComp.pd > 0) //new powerdraw not found so add
			externalComponents.push_back(extComp);

	}

	for (ExternalComponent extComp : externalComponents)
	{
		// The use adds the number of Watts/Joules as annotation to an external component that is abstracted as a declared "Energy function".
		// An expression in Watt is the energy consumption and equals to the number of Joules a component requires per second. In other words,
		// 1 Joule = 1 Watt-second. 

		// the powerdraw is the number of Watts multiplied with the number of seconds an instruction requires. However the instruction costs are expressed in microseconds and equal 1/1.000.000 of a second.
		long double powerdraw = extComp.pd * MICROSECONDS_TO_SECONDS(IC.InstrCost);
		AddJoules(powerdraw);
		AddExecTime(IC.InstrCost);
		log.LogDebug(" ==> Powerdraw: " + extComp.name.str() + ": " + std::to_string(extComp.pd) + "W  * " + std::to_string(MICROSECONDS_TO_SECONDS(IC.InstrCost)) + " = " + std::to_string(powerdraw) + "\n");
		

		
		//Adds the consumption to the component
		auto found = std::find_if(TEC.externalComponents.begin(), TEC.externalComponents.end(), [&extComp](const ExternalComponent& e) { return e.name == extComp.name;  });
		if (found != TEC.externalComponents.end())
		{
			//Optimization by storing the last powerdraw and execution time in the last visited BB
			// When the next BB will have the same name this code is not executed anymore
			// the function TotalEnergyConsumption::AddComponentConsumption makes use of this data
			ExternalComponent tempComp = *found;
			tempComp.energyConsumption.Joules = powerdraw;
			tempComp.energyConsumption.executionTime = IC.InstrCost;
			TECLastBB.externalComponents.push_back(tempComp);
			
			found->energyConsumption.Joules += powerdraw;
			found->energyConsumption.executionTime += IC.InstrCost;
		}
		
	}

	//log.LogConsole("total: " + std::to_string(TEC.energyConsumption.executionTime) + "\n");
	
}

//Can check if a function has energy annotation and will make sure the consumption is added to the totals.
//This function calls AddPowerdraw
bool  EnergyCalculation::SetEnergyFunctionCost(const llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I)
{
	if (&F != nullptr && HasEnergyAnnotation(F))
	{
		if (m_firstRun)
			m_firstRun = false;

		ExternalComponent extComp = GetEnergyValue(F);
	

		log.LogDebug("  EnergyFunction Name = " + extComp.name.str() + " pd = " + std::to_string(extComp.pd) + " ec = " + std::to_string(extComp.ec) + " wcet = " + std::to_string(USERDELAY_TO_MICROSECONDS(extComp.t)) + " microseconds\n");
	

		// The time given by the user is always in miliseconds and NOT in seconds or microseconds. The conversion is easy:
		// 1 millisecond = 0.001 second = 1000탎.
		// we get the energy consumption in Joules which is Watt per second. So a 60 Watt light bulb would consume 60 Joules per second
		// Because the delay by the user is given in miliseconds like 100 we must first divide t to microseconds (times 1000).
		// the reason is because we also calculate instruction costs per microsecond (탎). so the 100 milisecond delay is multiplied by 1000
		// and becomes 100.000탎 and is stored as such.

		//Example:
		// If something takes 5 seconds that means 5.000.000탎
		// when we calculate the number of Joules we must convert it back to seconds. We do this by dividing the total number of 탎 by 1.000.000
		// and mulitply that number with the number of Joules.
		//I.e. we have 120.000.000탎 = 120 seconds (120000 miliseconds) for a lightbulb of 60Watt this means 60*120 = 7200 Joules

		if (extComp.HasValues())
		{
			//Add the non temperal energy consumption to the number of joules
			AddJoules(extComp.ec);
			
			//Add the component to the unique list.
			//Function will take if it already exists or not
			AddExternalComponent(extComp);

			//Create an instructionCost class of the component
			// make sure that the user entered delay is converterd (default miliseconds to microseconds)
			InstructionCost IC = InstructionCost(&I, &BB, USERDELAY_TO_MICROSECONDS(extComp.t));

			//log.LogConsole(extComp.name.str() + ": " + std::to_string(IC.InstrCost) + "\n");
			// This is a toggle:
			// when the current energy function has a powerdraw the cost of the function itself
			// should not count as a powerdraw because that is captured by the ec value
			// When the powerdraw == 0 it is a stop function and must be added first 
			// before calcuting powerdraw to prevent the stop function itself to be counted 
			// as powerdraw too
			if (extComp.pd > 0)
			{
				AddPowerdraw(IC);
				TEC.externalComponentsTrace.push_back(extComp);
				TECLastBB.externalComponentsTrace.push_back(extComp);
			}
			else
			{
				
				TEC.externalComponentsTrace.push_back(extComp);
				TECLastBB.externalComponentsTrace.push_back(extComp);
				AddPowerdraw(IC);
			}

		}

		return true;
	}
	return false;
}


//Prints the final outcome of the calculation
void EnergyCalculation::Print()
{
	log.LogConsole("\n\n==================== Consumption per External power consuming component ===================\n\n");
	for (ExternalComponent& c : TEC.externalComponents)
	{
		
		std::cout << "             Name: " + c.name.str() << std::endl;
		std::cout << "           Joules: " << std::lround(c.energyConsumption.Joules) << std::endl;
		std::cout << "       Total Time: " << std::lround(MICROSECONDS_TO_SECONDS(c.energyConsumption.executionTime)) << " seconds\n";
		std::cout << "  Calculated Watt: " << std::lround(c.energyConsumption.Joules / MICROSECONDS_TO_SECONDS(c.energyConsumption.executionTime)) << " Watt\n\n";

	}

	log.LogConsole("================================= Total Energy Consumption ================================\n\n");
	
	std::cout << "         Total Cycles: " << std::lround(TEC.energyConsumption.numberOfCycles) << "\n\n";
	std::cout << "  Total ExecutionTime: " << std::lround(MICROSECONDS_TO_SECONDS(TEC.energyConsumption.executionTime)) << " seconds\n\n";
	std::cout << "         Total Joules: " << std::lround(TEC.energyConsumption.Joules) << "\n";

	log.LogConsole("\n===========================================================================================\n\n");

}