#include "EnergyCalculation.h"

int EnergyCalculation::visit(EnergyModule & em)
{

	log.LogConsole("Calculating Energy Consumption...");
	module = &em.GetLLVMModule();
	if (!CreateBBTrace())
		return EnergyAnalysis::E_MESSAGE_ERROR_PARSE_TRACEFILE;

	auto it = m_pathMap.begin();
	OrderedBBSet bbSet = it->second;

	if (!TraverseTrace(bbSet))
		return EnergyAnalysis::E_MESSAGE_ERROR_PARSE_TRACEFILE;

	log.LogConsole("Ok\n");
	return 0;
}

void EnergyCalculation::PrintTrace()
{
	for (TraceMap::iterator it = m_traceMap.begin(), ie = m_traceMap.end(); it != ie; ++it)
	{
		log.LogConsole(it->first.str() + ":\n");
		for (llvm::StringRef& bb : it->second)
			log.LogConsole(" " + bb.str());
		log.LogConsole("\n");
	}
}

void EnergyCalculation::Print()
{
	for (PathMap::iterator it = m_pathMap.begin(), ie = m_pathMap.end(); it != ie; ++it)
	{
		llvm::Function* fn = it->first;
		log.LogConsole(fn->getName().str() + ":\n");
		OrderedBBSet bbSet = it->second;
		for (llvm::BasicBlock* BB : bbSet)
			log.LogConsole(" " + getSimpleNodeLabel(BB));

		log.LogConsole("\n");
	}
}

bool EnergyCalculation::CreateBBTrace()
{

	for (llvm::Function& F : *module)
	{
		llvm::StringRef fnName = F.getName();
		if (m_traceMap.find(fnName) != m_traceMap.end())
		{
			if (m_pathMap.find(&F) == m_pathMap.end())
				m_pathMap[&F];

			std::vector<llvm::StringRef> nodeIDs = m_traceMap[fnName];
			for (llvm::StringRef& trID : nodeIDs)
			{
				for (llvm::BasicBlock& BB : F)
				{
					std::string bbID = getSimpleNodeLabel(&BB).substr(1);
					if (bbID.empty())
						continue;
					if (bbID == trID.str()) {
						m_pathMap[&F].push_back(&BB);
						break;
					}
						
				}
				
			}

			//if the following condition is true the count of both maps are not equal
			// thus something must have gone wrong.
			if (m_pathMap[&F].size() != m_traceMap[fnName].size())
				return false;
		}
		
	}
		
	return true;
}

bool EnergyCalculation::TraverseTrace(OrderedBBSet& OBB)
{
	for (llvm::BasicBlock* BB : OBB)
	{
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
				//SetInstructionCost(*F, *BB, inst);
				continue;
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
					if (m_pathMap.find(fn) == m_pathMap.end())
						return false; // the function is not in the trace. Something is wrong

					OrderedBBSet bbSet = m_pathMap[fn];
					if (!TraverseTrace(bbSet))
						return false; // something went wrong in the recursion
				}

			}
		}

	}
	return true;
}

//Todo finish this function
void EnergyCalculation::SetInstructionCost(llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I)
{
	InstructionCost IC = wcetAnalysis->GetInstructionCost(F, BB, I);
	//AddWCET(IC.InstrCost);
	log.LogDebug(" cost: " + std::to_string(IC.InstrCost) + "\n");
	//AddPowerdraw(IC);

}

//TODO: calculate the cost of Energy Function
bool  EnergyCalculation::SetEnergyFunctionCost(const llvm::Function& F, llvm::BasicBlock& BB, llvm::Instruction& I)
{
	if (&F != nullptr && HasEnergyAnnotation(F))
	{

	//	EnergyValue ev = GetEnergyValue(F);

	//	log.LogDebug("  EnergyFunction Name = " + ev.name.str() + " pd = " + std::to_string(ev.pd) + " ec = " + std::to_string(ev.ec) + " wcet = " + std::to_string(ev.t) + "\n");

	//	if (ev.HasValues())
	//	{
	//		// We consider Time t in this case equal to a cycle
	//		// Add the wcet of the energy function to the total WCET per Level
	//		AddWCET(ev.t);

	//		//Add the non temperal energy consumption to the number of joules per level
	//		AddJoules(ev.ec);

	//		InstructionCost IC = InstructionCost(&I, &BB, ev.t);

	//		// Add the Energy fucntion to the list of current functions of the current NestedLevel
	//			// and the current ProgramPath

	//		// This is a toggle
	//		// when the current energy function has a powerdraw the cost of the function itself
	//		// should not count as a powerdraw because that is captured by the ec value
	//		// When the powerdraw == 0 it is a stop function and must be added first 
	//		// before calcuting powerdraw to prevent the stop function itself to be counted 
	//		// as powerdraw too
	//		if (ev.pd > 0)
	//		{
	//			AddPowerdraw(IC);
	//			ProgramPathCosts[programPathID].energyConsumption[m_nestedLevel].push_back(ev);
	//		}
	//		else
	//		{
	//			ProgramPathCosts[programPathID].energyConsumption[m_nestedLevel].push_back(ev);
	//			AddPowerdraw(IC);
	//		}
	//	}
		return true;
	}
	return false;
}