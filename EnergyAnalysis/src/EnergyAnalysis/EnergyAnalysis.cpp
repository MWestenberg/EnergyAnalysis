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
		case E_MESSAGE_INVALID_ENTRY_POINT:  return ExitProgram(Error, std::string(ErrorMessages::MESSAGE_INVALID_ENTRY_POINT) + MODULE_ENTRY_POINT); break;

		default: return 0; break;
	}
	std::cin.get();
	return 0;
}

int EnergyAnalysis::ExitProgram(int Error, const std::string& message)
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
	
	//Run the Analysis
	log.LogInfo("=============================== Starting Energy Analysis ===============================\n\n");
	log.LogInfo("LLVM IR FILE: " + std::string(m_inputFile) + "\n\n");
	std::unique_ptr<EnergyModule> energy(new EnergyModule(*Mod));

	log.LogInfo("Reading Energy Annotations...\n");
	AnnotationVisitor annotate;
	energy->accept(annotate);

	log.LogInfo("Analysing Paths in CFG...\n");
	PathAnalysisVisitor pathAnalysis;
	energy->accept(pathAnalysis);

	log.LogInfo("Analysing Loops...\n");
	// The heap object will be deleted by the LLVM PassManager clean up
	LoopAnalysisVisitor loopAnalysis;
	energy->accept(loopAnalysis);
	loopAnalysis.Print();

	return NO_ERRORS;
}

bool EnergyAnalysis::IsValidBitcodeFile(const char * filename)
{

	//check if LLVM IR file is given as second argument
	std::ifstream infile(filename);
	return infile.good();
	
}
