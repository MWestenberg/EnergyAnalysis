#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "iostream"
#include "llvm/Pass.h"

#include "llvm/IR/InstIterator.h"
#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/LoopInfo.h>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/LegacyPassManagers.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include <llvm/Bitcode/BitcodeReader.h>
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"


using namespace llvm;

std::string getSimpleNodeLabel(const llvm::BasicBlock* Node) 
{

	if (!Node->getName().empty())
		return Node->getName().str();

	std::string Str;
	llvm::raw_string_ostream OS(Str);

	Node->printAsOperand(OS, false);
	return OS.str();
}

class BBinLoops : public PMDataManager, public FunctionPass, public PMTopLevelManager
{
	Function *F;
	LoopInfo *m_loopInfo;
public:

	static char ID; // Class identification, replacement for typeinfo
	BBinLoops() : FunctionPass(ID), PMDataManager(), PMTopLevelManager(new FPPassManager()), F(nullptr), m_loopInfo(nullptr) {
		llvm::initializeLoopInfoWrapperPassPass(
			*PassRegistry::getPassRegistry());
	}

	PMDataManager *getAsPMDataManager() override { return this; }
	Pass *getAsPass() override { return this; }

	PassManagerType getTopLevelPassManagerType() override {
		return PMT_BasicBlockPassManager;
	}

	FPPassManager *getContainedManager(unsigned N) {
		assert(N < PassManagers.size() && "Pass number out of range!");
		FPPassManager *FP = static_cast<FPPassManager *>(PassManagers[N]);
		return FP;
	}

	void getAnalysisUsage(AnalysisUsage &AU) const override {
		AU.setPreservesAll();
		AU.addRequired<LoopInfoWrapperPass>();

	}

	bool runOnFunction(Function &F) override {
		LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
		errs().write_escaped(F.getName());
		m_loopInfo = &LI;
		
		for (llvm::LoopInfo::iterator i = LI.begin(), e = LI.end(); i != e; ++i)
		{
			llvm::Loop* L = *i;
			L->dump();
		}


		return false;
	}

	LoopInfo* getLoopInfo() {
		return m_loopInfo;
	}

};

char BBinLoops::ID = 0;
//static RegisterPass<BBinLoops> X("BBinLoops", "test");





int main(int argc, char *argv[]) {
	StringRef filename = "C:/Dev/EnergyAnalysis/examples/plateCutter.ll";
	LLVMContext context;

	ErrorOr<std::unique_ptr<MemoryBuffer>> fileOrErr =
		MemoryBuffer::getFileOrSTDIN(filename);
	if (std::error_code ec = fileOrErr.getError()) {
		std::cerr << " Error opening input file: " + ec.message() << std::endl;
		return 2;
	}
	Expected<std::unique_ptr<Module>> moduleOrErr =
		parseBitcodeFile(fileOrErr.get()->getMemBufferRef(), context);
	if (std::error_code ec = fileOrErr.getError()) {
		std::cerr << "Error reading Module: " + ec.message() << std::endl;
		return 3;
	}

	llvm::SMDiagnostic Err;
	llvm::LLVMContext Context;
	std::unique_ptr<llvm::Module> m(parseIRFile(filename, Err, Context));
	if (!m)
		return 4;

	BBinLoops* bbs = new BBinLoops();

	legacy::PassManager pass;
	pass.add(bbs);
	pass.run(*m);




	
	return 0;
}