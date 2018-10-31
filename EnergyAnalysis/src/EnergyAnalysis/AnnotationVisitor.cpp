#include "AnnotationVisitor.h"

void AnnotationVisitor::visit(EnergyModule & em)
{
	llvm::Module& module = em.GetLLVMModule();
	// Get the real functions
	SetEnergyFunctions(module);
	AddAnnotation(module);
	//PrintAnnotations(module);
}



// Finds all global Energy Annotations and adds them to the real functions
// Due to limitations the Energy annotations are annoted to all functions by adding a underscore in front of the function names
// This methods reattaches those annotations to the correct functions to make use easier in a later stage.
void AnnotationVisitor::AddAnnotation(llvm::Module &M) {
	auto global_annos = M.getNamedGlobal("llvm.global.annotations");

	if (global_annos) {
		auto a = llvm::cast< llvm::ConstantArray>(global_annos->getOperand(0));
		for (unsigned int i = 0; i < a->getNumOperands(); i++) {
			auto e = llvm::cast< llvm::ConstantStruct>(a->getOperand(i));

			if (auto fn = llvm::dyn_cast<llvm::Function>(e->getOperand(0)->getOperand(0))) {
				llvm::StringRef anno = llvm::cast< llvm::ConstantDataArray>(llvm::cast< llvm::GlobalVariable>(e->getOperand(1)->getOperand(0))->getOperand(0))->getAsCString();
				std::vector<llvm::StringRef> tokens;
				Tokenize(tokens, anno, ',');

				//std::vector<llvm::StringRef> tokens = Tokenize(anno, ',');

				if (tokens.size() == EnergyAnalysis::NUM_OF_TOKENS) {
					assert(tokens.size() >= 4 && "Need at least 4 tokens in the Energy Annotation");
					llvm::Function* realFN = GetEnergyFunction(*fn);
					if (realFN)
					{
						//llvm::errs() << "Real:" << realFN->getName() << "\n";
						realFN->addFnAttr("Energy");
						int count = 1;

						for (llvm::StringRef token : tokens) {
							llvm::StringRef kind;
							switch (count) {
							case 1: kind = llvm::StringRef("name"); break;
							case 2: kind = llvm::StringRef("pd"); break;
							case 3: kind = llvm::StringRef("ec"); break;
							case 4: kind = llvm::StringRef("t"); break;
							}

							realFN->addFnAttr(kind, token.str());
							count++;
						}

						//remove dummy function
						RemoveUnusedFunctions(*fn);

					}
				}


				// So now we could more annotations if we would like.
				//                    fn->addFnAttr("Energy"); // <-- add function annotation here
				//                    fn->addFnAttr(anno);

			}
		}
		m_annotationedAdded = true; //for printing
	}
}


// Splits StringRef into separate tokens based on a separator.
// All annotations are comma separated in a certain order.
// Requires a reference to a vector
void AnnotationVisitor::Tokenize(std::vector<llvm::StringRef>& tokens, const llvm::StringRef& text, char sep)
{
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != llvm::StringRef::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
}

// Makes a map of all functions are real
// The functions that start with underscore are dummy functions added by the energy header to fool the compiler.
// If this code was not added the global annotations are removed by the frontend compiler
void AnnotationVisitor::SetEnergyFunctions(llvm::Module &M)
{

	for (llvm::Function& fn : M)
	{
		llvm::StringRef fnName = fn.getName();
		if (!fnName.startswith_lower("_"))
		{
			llvm::errs() << fnName.str() << "\n";
			m_EnergyFunctions[fn.getName()] = &fn;
		}

	}
}

void AnnotationVisitor::RemoveUnusedFunctions(llvm::Function& fn)
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

	if (m_EnergyFunctions.find(origName) != m_EnergyFunctions.end())
	{
		realFn = m_EnergyFunctions[origName];
	}

	return realFn;
}

// To test if the annotations are there this functions prints them to the console for all global annotated
// functions
void AnnotationVisitor::PrintAnnotations(llvm::Module& M) const
{

	if (!m_annotationedAdded) // the annotation was not added no use in running this function
		return;

	for (llvm::Function &fn : M) {

		if (fn.hasFnAttribute("Energy")) {
			std::string name = "";
			signed int pd = 0;
			unsigned int ec = 0;
			unsigned int t = 0;

			if (fn.hasFnAttribute("name")) {

				name = std::string(fn.getFnAttribute("name").getValueAsString());
			}
			if (fn.hasFnAttribute("pd")) {

				pd = std::stoi(fn.getFnAttribute("pd").getValueAsString().str());
			}
			if (fn.hasFnAttribute("ec")) {
				ec = std::stoi(fn.getFnAttribute("ec").getValueAsString().str());
			}
			if (fn.hasFnAttribute("t")) {
				t = std::stoi(fn.getFnAttribute("t").getValueAsString().str());
			}
			
			llvm::errs() << fn.getName() << " has Energy values: Time-dependent consumption=" << pd << " One-time energy consumption=" << ec << " Time=" << t << "!\n";

		}
	}
}