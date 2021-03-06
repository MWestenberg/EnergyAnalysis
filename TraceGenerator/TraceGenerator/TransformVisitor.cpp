#include "TransformVisitor.h"

int TransformVisitor::visit(TraceModule & em)
{
	log.LogConsole("Transforming code for partial execution...");
	module = &em.GetLLVMModule();
	AddPrintFunction();
	if (log.GetLevel() == Log::DEBUG)
		Print();

	log.LogConsole("Ok\n");
	return 0;
}


void TransformVisitor::WriteBitCodeFile(const llvm::StringRef & path)
{
	log.LogConsole("Writing bitcode file... ");
	std::error_code EC;
	llvm::raw_fd_ostream OS(path, EC, llvm::sys::fs::F_None);
	llvm::WriteBitcodeToFile(*module, OS);
	OS.flush();
	log.LogConsole("Ok\n");
}

void TransformVisitor::Print()
{
	module->print(llvm::outs(), nullptr);
}


void TransformVisitor::AddPrintFunction()
{
	llvm::LLVMContext& context = module->getContext();
	//llvm::IRBuilder<> builder(context);


	llvm::Constant *CalleeF = module->getOrInsertFunction("printf",
		llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(context),
			llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0),
			true /* this is var arg func type*/)
	);

	llvm::Function* printf = llvm::cast<llvm::Function>(CalleeF);
	//llvm::Function *printf = module->getFunction("printf");
	printf->setCallingConv(llvm::CallingConv::C);
	if (printf == nullptr) {
		log.LogError("no printf function");
		return;
	}

	for (llvm::Function& F : *module)
	{
		if (!IsNotTraversable(F))
		{
			for (llvm::BasicBlock& BB : F)
			{
				// initialize builder
				llvm::IRBuilder<> builder(&BB);
				//get the nodelabel
				std::string bbName = getSimpleNodeLabel(&BB);

				// create a string with functionname, nodelabel
				std::string  strVal = "F:" + F.getName().str() + std::string(",BB:") + bbName + ";";
				
				//create a name for the globalstr identifier (starts with .str)
				std::string globalStrID = std::string(".str") + bbName;
				//add the global string
				llvm::Value *strPtr = builder.CreateGlobalStringPtr(strVal, globalStrID);

				//A vector to hold all arguments
				std::vector<llvm::Value *> ArgsV;

				// Add all arguments of the printf function
				for (llvm::Function::arg_iterator argI = printf->arg_begin(), argIE = printf->arg_end(); argI != argIE; ++argI)
					ArgsV.push_back(argI);

				//replace the first argument with the globalstring pointer
				ArgsV[0] = strPtr;

				//get the last instruction of the basic block
				llvm::Instruction* inst = BB.getTerminator();
				//insert the print function
				llvm::CallInst::Create(printf, ArgsV, bbName.substr(1), inst);

			}
		}
	}

}