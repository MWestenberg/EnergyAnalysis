#include "EnergyAnalysis.h"


//overloaded operator only for this translation unit
static bool operator==(std::string& lhs, std::string& rhs)
{
	if (lhs.compare(rhs) == 0)
		return true;

	return false;
}


int EnergyAnalysis::ExitProgram(int Error)
{
	switch (Error)
	{
		case E_USAGE: return ExitProgram(Error, ErrorMessages::USAGE); break;
		case E_MESSAGE_MISSING_IRFILE: return ExitProgram(Error, ErrorMessages::MESSAGE_MISSING_IRFILE); break;
		case E_MESSAGE_INVALID_PASS:  return ExitProgram(Error, ErrorMessages::MESSAGE_INVALID_PASS); break;
		default: break;
	}
	std::cin.get();
	return 0;
}

int EnergyAnalysis::ExitProgram(int Error, const char* message)
{

	std::cout << std::endl;
	std::cout << "====================================================================================" << std::endl;
	std::cout << " The program exited with the following Error  " << std::endl;
	std::cout << " Error code:" << Error << std::endl;
	std::cout << std::endl;
	std::cout << " " << message << std::endl;
	std::cout << "====================================================================================" << std::endl;
	std::cout << std::endl;
	return Error;
}

int EnergyAnalysis::CheckArguments(int argc, char** argv)
{

	m_executable = const_cast<char*>(argv[0]);
	m_arg_counter = 0;
	m_argv = argv;

	//Missing Arguments, minimal 1 is required
	if (argc <= EnergyAnalysis::SHOWHELP)
	{
		return EnergyAnalysis::E_USAGE;// EnergyAnalysis::ExitProgram(EnergyAnalysis::E_USAGE);
	}

	//Check the first argument
	m_arg_counter ++;
	bool valid_file = false;

	//Check first argument is file and valid
	if (valid_file = EnergyAnalysis::IsValidBitcodeFile(const_cast<char*>(argv[1])))
	{
		//check if the argument is requesting help
		m_arg_counter++;
		
		//return EnergyAnalysis::ExitProgram(EnergyAnalysis::E_MESSAGE_MISSING_IRFILE);
	}

	for (; m_arg_counter < argc; m_arg_counter++)
	{
		EnergyAnalysis::ProgramOptions arg = EnergyAnalysis::AssignParam(argv[m_arg_counter]);
		switch (arg)
		{
		case EnergyAnalysis::RUN_PASS:
			if (valid_file)
				return EnergyAnalysis::NO_ERRORS;
			break;
		default: return EnergyAnalysis::SHOWHELP; // EnergyAnalysis::ExitProgram(EnergyAnalysis::SHOWHELP);
			break;
		}
		
	}

	return EnergyAnalysis::E_USAGE;
}

EnergyAnalysis::ProgramOptions EnergyAnalysis::AssignParam(const char * argument)
{
	if (strncmp(argument, "-h", 2) == 0) return SHOWHELP;
	if (strncmp(argument, "--help", 6) == 0) return SHOWHELP;
	if (strncmp(argument, "-pass", 5) == 0) return RUN_PASS;
	return SHOWHELP;
}

int EnergyAnalysis::SetInputFile()
{
	
	const char* argument = m_argv[m_arg_counter];
	m_inputFile = m_argv[1];

	//Check of the argument contains an equal sign
	std::string passArg = std::string(argument);
	bool containsEqual = passArg.find("=") != std::string::npos;
	if (!containsEqual)
	{
		return E_MESSAGE_INVALID_PASS;

	}
	

	// When we allow multiple parameters we can set them here.
	//m_EnergyAnalysisPass = nullptr;
	std::string modulePass = passArg.substr(passArg.find("=") + 1);
	if (modulePass.compare(std::string(AnalysisType::ANNOTATION)) == 0) // returns 0 if equal
	{
			return NO_ERRORS;
	}
	
	return E_MESSAGE_INVALID_PASS;

}

int EnergyAnalysis::StartEnergyAnalysis()
{
	//Prepare Module 
	llvm::SMDiagnostic Err;
	llvm::LLVMContext Context;
	std::unique_ptr<llvm::Module> Mod(llvm::parseIRFile(m_inputFile, Err, Context));
	if (!Mod)
		return E_MESSAGE_ERROR_IRFILE;
	
	/*
	ik heb een module die bestaat uit functies en bb
	elke functie bestaat uit alleen instructies of bb.
	elke bb bestaat uit instructies

	stap 1: we moeten weten in welke volgorde de functies worden aangeroepen die energie verbruiken
	daarvoor moet eerst elke functie een energienotatie krijgen. Dit werkt al.

	stap 2: Nu moeten alle functies die aangeroepen worden in de juiste volgorde worden gezet.
	Dit kan door deze in een multiset te zetten std::multiset<llvm::StringRef, llvm::Function); waarbij de StringRef gelijk is aan llvm::Function.GetName()

	stap 3: per functie moet de volgorde worden bepaald en de SCC's worden bepaald. Ook hiervoor maken we een order set van BB's per functie maken. Als we dit doen kunnen we natuurlijk ook meteen voor de functie de kosten per pad uitrekenen en het duurste pad opslaan

	Stap 4: ...
	*/

	//Run the Analysis
	std::unique_ptr<EnergyModule> energy(new EnergyModule(*Mod));
	AnnotationVisitor annotate;
	energy->accept(annotate);

	TopoSorter topoSorter;
	energy->accept(topoSorter);

	CallGraphVisitor cfg;
	energy->accept(cfg);



	/*AnnotationPass annotate;
	annotate.runOnModule(*m_Module);
	annotate.PrintAnnotations(*m_Module);*/


	
	return NO_ERRORS;
}

bool EnergyAnalysis::IsValidBitcodeFile(const char * filename)
{

	//check if LLVM IR file is given as second argument
	std::ifstream infile(filename);
	return infile.good();
	
}
