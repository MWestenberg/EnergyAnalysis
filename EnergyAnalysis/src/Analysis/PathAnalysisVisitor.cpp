#include "PathAnalysisVisitor.h"

int PathAnalysisVisitor::visit(EnergyModule & em)
{
	log.LogConsole("Analysing Paths in CFG...");
	llvm::Module& module = em.GetLLVMModule();
	// This just gets the main entry point from the Module.
	/*llvm::Function *mainFunction = GetModuleEntryPoint(module);
	if (mainFunction == nullptr)
	{
		EnergyAnalysis::ExitProgram(EnergyAnalysis::E_MESSAGE_INVALID_ENTRY_POINT);
		return;
	}*/

	for (llvm::Function& fn : module) {
		ProfilePath(fn);
	}

	log.LogConsole("Ok\n");
	return 0;
}

bool PathAnalysisVisitor::ProfilePath(llvm::Function & F)
{

	// Energy functions, LOOP_TRIPCOUNT and undeclared functions should be ignored
	if (IsNotTraversable(F))
		return false;

	log.LogDebug("\n================== Analysis of Function: " + F.getName().str() + "===================\n");

	// Find backedges that initiate a loop
	SetBackEdges(F);

	// Entry Block of function
	llvm::BasicBlock& entryBB = F.getEntryBlock();

	// Reset hte PathMap
	pathID = 0;
	paths.shrink_and_clear();

	//create paths
	CreatePath(entryBB);
	fp[&F] = paths;

	log.LogDebug("================================================================\n");
	
	return true;
}


void PathAnalysisVisitor::CreatePath(llvm::BasicBlock& bb)
{
	log.LogDebug("PathID: " + std::to_string(pathID) +" add: " + getSimpleNodeLabel(&bb) + "\n");

	//what is the current path? pathID
	// get the pathmap
	if (paths.size() == 0)
	{
		//first stop
		OrderedBBSet vertices;
		vertices.push_back(&bb);
		paths[pathID] = vertices;
	}
	else
	{
		OrderedBBSet* vertices = &paths[pathID];
		vertices->push_back(&bb);
	}

	//retrieve the terminating instruction of the current basic block
	const llvm::TerminatorInst *TInst = bb.getTerminator();

	if (TInst->getNumSuccessors() == 0) {
		log.LogDebug("===== End of path " + std::to_string(pathID) + "==========\n");
		log.LogDebug("Path Summary: ");
		for (auto sb : paths[pathID])
		{
			log.LogDebug(getSimpleNodeLabel(sb) + " ");
		}

		log.LogDebug("\n=======================================================\n");

	}

	int NSucc = TInst->getNumSuccessors();
	//if (NSucc == 1)
	//	CreatePath(*TInst->getSuccessor(0));

	bool firstEdgeIsLoop = false;

	//Iterate over all successors of the Basic block
	for (int i = 0; i < NSucc; i++)
	{
		log.LogDebug("\n====Loop of root bb: " + getSimpleNodeLabel(&bb) + "===\n");

		llvm::BasicBlock *Succ = TInst->getSuccessor(i);
		if (std::find(paths[pathID].begin(), paths[pathID].end(), Succ) != paths[pathID].end())
		{
			Edge edge(&bb, Succ);
			
			auto foundEdge = std::find(loopEdges.begin(), loopEdges.end(), edge);
			if (foundEdge != loopEdges.end())
			{
				Edge e = *foundEdge;
				if (log.GetLevel() == log.DEBUG)
					e.Print();

				// BB's always have max 2 branches, True and False
				// in case a Loop was found in the first branch (true)
				// the second must be followed and added to the same path.
				if (i == 0)
					firstEdgeIsLoop = true;
				continue;
			}
				
		}

		OrderedBBSet vertices = paths[pathID];

		if (!firstEdgeIsLoop)
			pathID += i;
		
		if (i > 0 && !firstEdgeIsLoop) {
			for (llvm::BasicBlock* hbb : vertices)
			{
				if (hbb == &bb)
					break;
				else
					paths[pathID].push_back(hbb);
			}
			printPath(paths[pathID]);
			paths[pathID].push_back(&bb);
			log.LogDebug(" add: " + getSimpleNodeLabel(&bb) + "\n");
		}

		CreatePath(*Succ);
	}


}

/*
Location of backedges, these are basically the loops that should be ignored for now.
examples: 
	results for main in platecutter is:
		%30 =>  %30
		%34 =>  %9

		and for loopbranch.ll on function loop:
		%25 =>  %6
*/
void PathAnalysisVisitor::SetBackEdges(const llvm::Function& F)
{
	llvm::SmallVector< std::pair<const llvm::BasicBlock *, const llvm::BasicBlock *>, 32> result;
	llvm::FindFunctionBackedges(F, result);

	typedef std::pair<const llvm::BasicBlock *, const llvm::BasicBlock *> BlockPair;
	for (llvm::SmallVectorImpl<std::pair<const llvm::BasicBlock *, const llvm::BasicBlock *> >::iterator I = result.begin(),
		IE = result.end(); I != IE; ++I)
	{
		Edge edge;
		edge.from = const_cast<llvm::BasicBlock*>(I->first);
		edge.to = const_cast<llvm::BasicBlock*>(I->second);
		edge.isLoop = true;
		loopEdges.push_back(edge);

		log.LogDebug(getSimpleNodeLabel(I->first) + " =>  " + getSimpleNodeLabel(I->second) + "\n");

	}
}


void PathAnalysisVisitor::printPath(const OrderedBBSet& path)
{
	if (log.GetLevel() != log.DEBUG)
		return;

	for (llvm::BasicBlock* b : path)
	{
		log.LogDebug(" add: " + getSimpleNodeLabel(b) + "(backtrace) \n");
	}
}


void PathAnalysisVisitor::Print()
{

	log.LogConsole(PRINT_BEGIN);
	
	if (fp.size() == 0) {
		log.LogConsole("There are no PathMaps. Did you run the visit method?");
		return;
	}

	for (auto I = fp.begin(), IE = fp.end(); I != IE; ++I)
	{
		llvm::Function& fn = *I->first;
		PathMap& pm = I->second;
		log.LogConsole("Function: " + fn.getName().str() + "\n");
		for (auto PI = pm.begin(), PIE = pm.end(); PI != PIE; ++PI)
		{
			log.LogConsole(" Path " + std::to_string(PI->first) + ":\n");
			OrderedBBSet bbSet = PI->second;
			for (auto sb : bbSet)
			{
				log.LogConsole("  " + getSimpleNodeLabel(sb) + "\n");
			}
			log.LogConsole("\n");

		}
		log.LogConsole("\n");
	}
	log.LogConsole(PRINT_END);
}