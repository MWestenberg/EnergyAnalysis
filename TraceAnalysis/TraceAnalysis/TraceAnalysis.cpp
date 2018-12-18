#include "TraceAnalysis.h"


int TraceAnalysis::ExitProgram(int Error)
{
	switch (Error)
	{
		case E_USAGE: return ExitProgram(Error, ErrorMessages::USAGE); break;
		case E_MESSAGE_MISSING_IRFILE: return ExitProgram(Error, ErrorMessages::MESSAGE_MISSING_IRFILE); break;
		case E_MESSAGE_ERROR_IRFILE: return ExitProgram(Error, ErrorMessages::MESSAGE_ERROR_IRFILE); break;
		case E_MESSAGE_INVALID_ENTRY_POINT:  return ExitProgram(Error, std::string(ErrorMessages::MESSAGE_INVALID_ENTRY_POINT) + MODULE_ENTRY_POINT); break;
		case E_MESSAGE_INVALID_TOKENS:  return ExitProgram(Error, ErrorMessages::MESSAGE_INVALID_TOKENS); break;
		default: return 0;  break;
	}

	std::cin.get();
	return 0;
}

int TraceAnalysis::ExitProgram(int Error, const std::string& message)
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

int TraceAnalysis::CheckArguments(int argc, char** argv)
{

	m_executable = const_cast<char*>(argv[0]);
	m_arg_counter = 0;
	m_argv = argv;

	//Missing Arguments, minimal 1 is required
	if (argc <= TraceAnalysis::SHOWHELP)
	{
		return TraceAnalysis::E_USAGE;
	}

	//Check the first argument
	m_arg_counter++;
	bool valid_file = false;

	//Check first argument is file and valid
	if (valid_file = TraceAnalysis::IsValidBitcodeFile(const_cast<char*>(argv[1])))
	{
		//check if the argument is requesting help
		m_arg_counter++;

		//return EnergyAnalysis::ExitProgram(EnergyAnalysis::E_MESSAGE_MISSING_IRFILE);
	}

	for (; m_arg_counter < argc; m_arg_counter++)
	{
		TraceAnalysis::ProgramOptions arg = TraceAnalysis::AssignParam(argv[m_arg_counter]);
		switch (arg)
		{
		case TraceAnalysis::RUN_PASS:
			if (valid_file)
				return TraceAnalysis::NO_ERRORS;
			break;
		default: return TraceAnalysis::SHOWHELP;
			break;
		}

	}

	return TraceAnalysis::NO_ERRORS;
}

TraceAnalysis::ProgramOptions TraceAnalysis::AssignParam(const char * argument)
{
	if (strncmp(argument, "-h", 2) == 0) return SHOWHELP;
	if (strncmp(argument, "--help", 6) == 0) return SHOWHELP;
	return RUN_PASS;
	
}

int TraceAnalysis::SetInputFile()
{

	//const char* argument = m_argv[m_arg_counter];
	m_inputFile = m_argv[1];

	return NO_ERRORS;

	////Check of the argument contains an equal sign
	//std::string passArg = std::string(argument);
	//bool containsEqual = passArg.find("=") != std::string::npos;
	//if (!containsEqual)
	//{
	//	return E_MESSAGE_INVALID_PASS;

	//}


	//// When we allow multiple parameters we can set them here.
	////m_EnergyAnalysisPass = nullptr;
	//std::string modulePass = passArg.substr(passArg.find("=") + 1);
	//if (modulePass.compare(std::string(AnalysisType::ANNOTATION)) == 0) // returns 0 if equal
	//{
	//	return NO_ERRORS;
	//}

	//return E_MESSAGE_INVALID_PASS;

}


int TraceAnalysis::StartTraceAnalysis()
{
	int Error = NO_ERRORS;

	//Prepare Module 
	llvm::SMDiagnostic Err;
	llvm::LLVMContext Context;
	std::unique_ptr<llvm::Module> Mod(llvm::parseIRFile(m_inputFile, Err, Context));
	if (!Mod)
		return E_MESSAGE_ERROR_IRFILE;


	//Run the Analysis
	log.LogConsole("=============================== Starting Energy Analysis ===============================\n\n");
	log.LogConsole("LLVM IR FILE: " + std::string(m_inputFile) + "\n\n");
	std::unique_ptr<TraceModule> energy(new TraceModule(*Mod));

	// Handles Energy Annotations implemented by the EnergyAnalysis.h header file
	std::unique_ptr<AnnotationTransformer> transformAnnotationCall(new AnnotationTransformer);
	Error = energy->accept(*transformAnnotationCall);
	if (Error)
		return ExitProgram(Error);
	
	std::unique_ptr<TransformVisitor> transform(new TransformVisitor);
	Error = energy->accept(*transform);
	if (Error)
		return ExitProgram(Error);

	transform->WriteBitCodeFile("../Tests/LightBulb/lightbulb.bc");
	
	
	return Error;
}

bool TraceAnalysis::IsValidBitcodeFile(const char * filename)
{

	//check if LLVM IR file is given as second argument
	std::ifstream infile(filename);
	return infile.good();

}
