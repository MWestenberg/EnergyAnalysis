#include "AnnotationTransformer.h"



int AnnotationTransformer::visit(TraceModule & em)
{
	log.LogConsole("Reading Energy Annotations...\n");
	module = &em.GetLLVMModule();
	// Get the real functions
	SetEnergyFunctions();
	if (!locateAnnotations())
		return m_result;
	log.LogConsole("Ok\n");

	return 0;
}

void AnnotationTransformer::Print()
{
	module->print(llvm::outs(), nullptr);
}


//this function locates all annotations and tries to find the declared function that belongs to it
// If found we know the declared function is an energy function.
// Because we call the declared function to fool the compiler not to change the code we must transform
// the call instruction with the defined version to allow for partial execution.
bool AnnotationTransformer::locateAnnotations()
{
	auto global_annos = module->getNamedGlobal(LLVM_GLOBAL_ANNOTATIONS);

	if (!global_annos)
		return true;

	auto a = llvm::cast< llvm::ConstantArray>(global_annos->getOperand(0));
	for (unsigned int i = 0; i < a->getNumOperands(); i++) {
		auto e = llvm::cast< llvm::ConstantStruct>(a->getOperand(i));

		if (auto fn = llvm::dyn_cast<llvm::Function>(e->getOperand(0)->getOperand(0)))
		{
			llvm::StringRef anno = llvm::cast< llvm::ConstantDataArray>(llvm::cast< llvm::GlobalVariable>(e->getOperand(1)->getOperand(0))->getOperand(0))->getAsCString();
			std::vector<llvm::StringRef> tokens;
			Tokenize(tokens, anno, ',');

			if (tokens.size() == TraceAnalysis::NUM_OF_TOKENS)
			{
				assert(tokens.size() >= 4 && "Need at least 4 tokens in the Energy Annotation");
				llvm::Function* realEnergyFn = GetEnergyFunction(*fn);
				if (realEnergyFn)
				{
					transformEnergyFunctionCalls(realEnergyFn, fn);
				}
			}
			else
			{
				log.LogError("Program halted");
				m_result = TraceAnalysis::E_MESSAGE_INVALID_TOKENS;
				return false;
			}
		}

	}
	return true;
}

bool AnnotationTransformer::transformEnergyFunctionCalls(llvm::Function* realEnergyFn, llvm::Function* definedFunction)
{

	for (llvm::Function& fn : *module)
	{
		for (llvm::BasicBlock& bb : fn)
		{
			for (llvm::Instruction& i : bb)
			{
				//cast instruction to callsite
				llvm::CallSite cs(&i);
				if (!cs.getInstruction())
					continue;

				llvm::CallInst * callInst = llvm::dyn_cast<llvm::CallInst>(const_cast<llvm::Instruction*>(&i));
				if (callInst == nullptr)
					continue;

				//at this point get the function pointer
				llvm::Function *func = llvm::dyn_cast<llvm::Function>(callInst->getCalledValue()->stripPointerCasts());
				if (func == nullptr)
					continue;

				log.LogConsole("found func: " + func->getName().str() + " EnergyFunc: " + realEnergyFn->getName().str() +" defined func: " + definedFunction->getName().str() +"\n");
				//if current callinstruction is pointing to an energyfunction
				// e.g. call to turnOn() replace that with a call to _turnOn();
				if (func != nullptr && func == realEnergyFn)
				{
					llvm::CallInst* NewCI = llvm::CallInst::Create(definedFunction, "");
					NewCI->setCallingConv(definedFunction->getCallingConv());

					//replace all uses with the new call instruction
					if (!i.use_empty())
						i.replaceAllUsesWith(NewCI);
					//replace the call with the new call 
					// e.g. replace turnOn() to _turnOn();
					llvm::ReplaceInstWithInst(&i, NewCI);

					//because a function was found the loop must break and reiterated until the next energy function is found
					//this is not a very efficient way but it prevents calling instructions that were changed.
					// using a normal iterator will not work
					transformEnergyFunctionCalls(realEnergyFn, definedFunction);
					// loop must be broken
					break;
				}

			}
		}
	}


	return true;
}


void AnnotationTransformer::Tokenize(std::vector<llvm::StringRef>& tokens, const llvm::StringRef & text, char sep) const
{
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != llvm::StringRef::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
}

void AnnotationTransformer::SetEnergyFunctions()
{

	for (llvm::Function& fn : *module)
	{
		llvm::StringRef fnName = fn.getName();
		std::string prefix = ENERGY_FUNCTION_PREFIX;
		if (!fnName.startswith_lower(prefix))
		{
			m_filteredFunctions[fn.getName()] = &fn;
		}

	}
}

// Retrieves the orignal Energy Annotated function based on a declared function
// Each Energy function is called in the code with their real name, however if only a 
// prototype is added of a function the annotation is removed by the front-end compiler.
// By declaring all external functions with an _ in front of it _<orignalName> we 
// can reannotate the definitions
llvm::Function * AnnotationTransformer::GetEnergyFunction(const llvm::Function & fn)
{
	llvm::StringRef origName = llvm::StringRef(fn.getName()).substr(1);
	llvm::Function* realFn = nullptr;

	if (m_filteredFunctions.find(origName) != m_filteredFunctions.end())
	{
		realFn = m_filteredFunctions[origName];
	}

	return realFn;
}
