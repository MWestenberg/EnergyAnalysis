#include "AnnotationVisitor.h"

int AnnotationVisitor::visit(EnergyModule & em)
{
	log.LogConsole("Reading Energy Annotations...");
	llvm::Module& module = em.GetLLVMModule();
	// Get the real functions
	SetEnergyFunctions(module);
	if (!AddAnnotation(module))
		return m_result;	
	log.LogConsole("Ok\n");
	
	return 0;
}


// Finds all global Energy Annotations and adds them to the real functions
// Due to limitations the Energy annotations are annoted to all functions by adding a underscore in front of the function names
// This methods reattaches those annotations to the correct functions to make use easier in a later stage.
bool AnnotationVisitor::AddAnnotation(llvm::Module &M) {
	auto global_annos = M.getNamedGlobal(LLVM_GLOBAL_ANNOTATIONS);

	if (!global_annos)
		return true;

	auto a = llvm::cast< llvm::ConstantArray>(global_annos->getOperand(0));
	for (unsigned int i = 0; i < a->getNumOperands(); i++) {
		auto e = llvm::cast< llvm::ConstantStruct>(a->getOperand(i));

		if (auto fn = llvm::dyn_cast<llvm::Function>(e->getOperand(0)->getOperand(0))) {
			llvm::StringRef anno = llvm::cast< llvm::ConstantDataArray>(llvm::cast< llvm::GlobalVariable>(e->getOperand(1)->getOperand(0))->getOperand(0))->getAsCString();
			std::vector<llvm::StringRef> tokens;
			Tokenize(tokens, anno, ',');

			if (tokens.size() == NUM_OF_TOKENS) 
			{
				assert(tokens.size() >= 4 && "Need at least 4 tokens in the Energy Annotation");
				llvm::Function* realEnergyFn = GetEnergyFunction(*fn);
				if (AddEnergyAttribute(*realEnergyFn, tokens))
					RemoveUnusedFunctions(*fn); //remove dummy function
			}
			else 
			{
				log.LogError("Program halted");
				m_result = EnergyAnalysis::E_MESSAGE_INVALID_TOKENS;
				return false;
			}
		}
	}
	
	
	return true;
}

bool AnnotationVisitor::AddEnergyAttribute(llvm::Function& F, const std::vector<llvm::StringRef>& tokens)
{
	if (&F)
	{
		//llvm::errs() << "Real:" << realFN->getName() << "\n";
		F.addFnAttr(ENERGY_ATTR);
		int count = 1;

		for (llvm::StringRef token : tokens) {
			llvm::StringRef kind;
			switch (count) {
			case 1: kind = llvm::StringRef(ENERGY_FUNCTION_NAME); break;
			case 2: kind = llvm::StringRef(ENERGY_TEMPORAL_CONSUMPTION); break;
			case 3: kind = llvm::StringRef(ENERGY_CONSUMPTION); break;
			case 4: kind = llvm::StringRef(ENERGY_TIME_UNIT); break;
			}

			F.addFnAttr(kind, token.str());
			count++;
		}
		m_annotationedAdded = true; //for printing
		return true;
	}
	return false;
}

// Makes a map of all functions are real
// The functions that start with underscore are dummy functions added by the energy header to fool the compiler.
// If this code was not added the global annotations are removed by the frontend compiler
void AnnotationVisitor::SetEnergyFunctions(llvm::Module &M)
{
	for (llvm::Function& fn : M)
	{
		llvm::StringRef fnName = fn.getName();
		std::string prefix = ENERGY_FUNCTION_PREFIX;
		if (!fnName.startswith_lower(prefix))
		{
			m_filteredFunctions[fn.getName()] = &fn;
		}
		

	}
}

void AnnotationVisitor::RemoveUnusedFunctions(llvm::Function& fn) const
{
	fn.replaceAllUsesWith(llvm::UndefValue::get((llvm::Type*)fn.getType()));
	fn.eraseFromParent();
}

// Retrieves the orignal Energy Annotated function based on a declared function
// Each Energy function is called in the code with their real name, however if only a 
// prototype is added of a function the annotation is removed by the front-end compiler.
// By declaring all external functions with an _ in front of it _<orignalName> we 
// can reannotate the definitions
llvm::Function* AnnotationVisitor::GetEnergyFunction(const llvm::Function& fn)
{
	llvm::StringRef origName = llvm::StringRef(fn.getName()).substr(1);
	llvm::Function* realFn = nullptr;

	if (m_filteredFunctions.find(origName) != m_filteredFunctions.end())
	{
		realFn = m_filteredFunctions[origName];
	}

	return realFn;
}

// To test if the annotations are there this functions prints them to the console for all global annotated
// functions
void AnnotationVisitor::Print(llvm::Module& M) 
{
	log.LogConsole("Energy Annotations: \n");

	if (!m_annotationedAdded) { // the annotation was not added no use in running this function
		log.LogWarning(" You cannot print the annotations before calling the visit method.\nIt might also be that the program you are trying to analyse doesn contain any annotations.\n");
		return;
	}
	for (llvm::Function &fn : M) {

		ExternalComponent extComp = GetEnergyValue(fn);

		if (extComp.HasValues())
			log.LogConsole(" " + extComp.name.str() + " has Energy values: Time-dependent consumption=" + std::to_string(extComp.pd) + " One-time energy consumption=" + std::to_string(extComp.ec) + " Time=" + std::to_string(extComp.t) + "\n");
	}

	log.LogConsole("\n");
}