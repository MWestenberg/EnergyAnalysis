#include "TraceGenerator.h"


int TraceGenerator::ExitProgram(int Error)
{
	switch (Error)
	{
		case E_USAGE: return ExitProgram(Error, ErrorMessages::USAGE); break;
		case E_MESSAGE_MISSING_IRFILE: return ExitProgram(Error, ErrorMessages::MESSAGE_MISSING_IRFILE); break;
		case E_MESSAGE_INVALID_ARGUMENT:  return ExitProgram(Error, ErrorMessages::MESSAGE_INVALID_ARGUMENT); break;
		case E_MESSAGE_ERROR_IRFILE:  return ExitProgram(Error, ErrorMessages::MESSAGE_ERROR_IRFILE); break;
		default: return 0;  break;
	}

	std::cin.get();
	return 0;
}

int TraceGenerator::ExitProgram(int Error, const std::string& message)
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

int TraceGenerator::CheckArguments(int argc, char** argv)
{

	m_executable = const_cast<char*>(argv[0]);
	m_arg_counter = 0;
	m_argv = argv;
	m_argc = argc;

	//Missing Arguments, minimal 1 is required
	if (argc <= TraceGenerator::SHOWHELP)
	{
		return TraceGenerator::E_USAGE;// 
	}

	//Check the first argument
	m_arg_counter++;
	int error = NO_ERRORS;

	for (; m_arg_counter < argc; m_arg_counter++)
	{
		error = ReadArgument(argv[m_arg_counter]);
		if (error)
			return error;
	}

	if (m_inputFile && !m_inputFile[0])
		error = E_MESSAGE_MISSING_IRFILE;

	if (m_outputFile && !m_outputFile[0])
		error = E_MESSAGE_INVALID_OUTPUT;

	if (error)
		return TraceGenerator::E_USAGE;

	return NO_ERRORS;
}


int TraceGenerator::ReadArgument(const char* argument)
{
	int error = NO_ERRORS;
	
	//help argument options
	if (strncmp(argument, "-h", 2) == 0 || strncmp(argument, "--help", 6) == 0)
		m_action = SHOWHELP;
	if (strncmp(argument, "-d", 2) == 0 || strncmp(argument, "--debug", 7) == 0)
		log.SetLevel(Log::DEBUG);
	else if (strncmp(argument, "-o", 2) == 0)
	{
		// output fiule option
		if (++m_arg_counter < m_argc)
			error = SetArgument(OUTPUTFILE, m_argv[m_arg_counter]);
	}
	else if (m_inputFile && !m_inputFile[0])
	{
		//input file (llvm IR file)
		if (IsValidFile(const_cast<char*>(argument)))
		{
			int error = SetArgument(IRFILE, argument);
			if (error)
				return error;
		}
	}
	else
	{
		//unknown parameter, ignore
	}

	return error;

}


int TraceGenerator::SetArgument(ArgType type, const char* value)
{
	int error = NO_ERRORS;
	switch (type)
	{
	case IRFILE: m_inputFile = const_cast<char*>(value);  break;
	case OUTPUTFILE: {
		m_outputFile = const_cast<char*>(value);
		if (!IsWriteable(m_outputFile))
			error = E_MESSAGE_INVALID_OUTPUT;
		break;
	}
	default: error = E_MESSAGE_INVALID_ARGUMENT; break;
	}
	return error;
}


int TraceGenerator::StartTraceGenerator()
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
	log.LogConsole("LLVM IR FILE: " + std::string(m_inputFile) + "\n");
	log.LogConsole("BITCODE OUTPUT FILE: " + std::string(m_outputFile) + "\n\n");
	std::unique_ptr<TraceModule> energy(new TraceModule(*Mod));

	// Handles Energy Annotations implemented by the EnergyAnalysis.h header file
	std::unique_ptr<AnnotationTransformer> transformAnnotationCall(new AnnotationTransformer(log.GetLevel()));
	Error = energy->accept(*transformAnnotationCall);
	if (Error)
		return ExitProgram(Error);
	
	std::unique_ptr<TransformVisitor> transform(new TransformVisitor(log.GetLevel()));
	Error = energy->accept(*transform);
	if (Error)
		return ExitProgram(Error);

	transform->WriteBitCodeFile(m_outputFile);
	
	log.LogConsole("\n\nTo create the trace run the following command (omit '> traceoutput.txt' for console output):\n\n");
	log.LogConsole(" lli --force-interpreter " + std::string(m_outputFile)+" > traceoutput.txt\n\n");
	log.LogConsole("When you know the cpu and it is installed you can run a command with cpu type.\nFor example for the Arduino UNO:\n\n");
	log.LogConsole(" lli --force-interpreter -march=avr -mcpu=atmega328p " + std::string(m_outputFile)+ " > traceoutput.txt\n\n");
	return Error;
}

bool TraceGenerator::IsValidFile(const char * filename)
{

	//check if file is valid
	std::ifstream infile(filename);
	return infile.good();

}


bool TraceGenerator::IsWriteable(const char* filename)
{
	std::ofstream ofs;
	ofs.open(filename);
	if (ofs.is_open())
	{
		ofs.close();
		return true;
	}
	else
	{
		return false;
	}
}