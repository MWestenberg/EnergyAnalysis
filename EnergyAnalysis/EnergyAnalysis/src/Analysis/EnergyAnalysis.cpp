#include "EnergyAnalysis.h"

//
////overloaded operator only for this translation unit
//static bool operator==(std::string& lhs, std::string& rhs)
//{
//	if (lhs.compare(rhs) == 0)
//		return true;
//
//	return false;
//}


int EnergyAnalysis::ExitProgram(int Error)
{
	switch (Error)
	{
		case E_USAGE: return ExitProgram(Error, ErrorMessages::USAGE); break;
		case E_MESSAGE_MISSING_IRFILE: return ExitProgram(Error, ErrorMessages::MESSAGE_MISSING_IRFILE); break;
		case E_MESSAGE_INVALID_ARGUMENT:  return ExitProgram(Error, ErrorMessages::MESSAGE_INVALID_ARGUMENT); break;
		case E_MESSAGE_INVALID_TOKENS:  return ExitProgram(Error, ErrorMessages::MESSAGE_INVALID_TOKENS); break;
		case E_MESSAGE_UNDEFINED_LOOP: return ExitProgram(Error, ErrorMessages::MESSAGE_UNDEFINED_LOOP); break;
		case E_MESSAGE_ERROR_PARSE_TRACEFILE: return ExitProgram(Error, ErrorMessages::MESSAGE_ERROR_PARSE_TRACEFILE); break;
		default: return 0;  break;
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
	m_argc = argc;

	//Missing Arguments, minimal 1 is required
	if (argc <= EnergyAnalysis::SHOWHELP)
	{
		return EnergyAnalysis::E_USAGE;// EnergyAnalysis::ExitProgram(EnergyAnalysis::E_USAGE);
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

	if (error)
		return EnergyAnalysis::E_USAGE;

	return NO_ERRORS;
}

int EnergyAnalysis::ReadArgument(const char* argument)
{
	int error = NO_ERRORS;

	//help argument options
	if (strncmp(argument, "-h", 2) == 0 || strncmp(argument, "--help", 6) == 0)
		m_action = SHOWHELP;
	if (strncmp(argument, "-d", 2) == 0 || strncmp(argument, "--debug", 7) == 0)
		log.SetLevel(Log::DEBUG);
	if (strncmp(argument, "-i", 2) == 0 || strncmp(argument, "--info", 6) == 0)
		log.SetLevel(Log::INFO);
	else if (strncmp(argument, "-clockspeed=", 12) == 0)
		error = SetArgument(CLOCKSPEED, std::string(argument).substr(12).c_str());
	else if (strncmp(argument, "-trace=", 7) == 0)
		error = SetArgument(TRACE, std::string(argument).substr(7).c_str());
	else if (strncmp(argument, "-o", 2) == 0)
	{
		// output fiule option
		if (++m_arg_counter < m_argc)
			error = SetArgument(OUTPUTFILE, m_argv[m_arg_counter]);
	}
	else if (m_inputFile &&  !m_inputFile[0])
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


int EnergyAnalysis::SetArgument(ArgType type, const char* value)
{
	int error = NO_ERRORS;
	switch (type)
	{
		case TRACE: {
			m_traceFile =  const_cast<char*>(value); 
			if (!IsValidFile(m_traceFile))
				error = E_MESSAGE_ERROR_TRACEFILE;

			ImportTrace();
			break;
		}
		case CLOCKSPEED: {
			m_clockspeed = std::stoi(value);
			if (!m_clockspeed) {
				error = E_MESSAGE_INVALID_CLOCKSPEED;
			}			
			break;
		}
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


int EnergyAnalysis::ImportTrace()
{

	std::ifstream ifs(m_traceFile);
	std::stringstream ss;

	if (ifs.is_open()) {
		char c = ifs.get();
		while (ifs.good()) {
			ss << c;
			c = ifs.get();
		}
	}
	
	if (ss.str().empty())
		return E_MESSAGE_ERROR_TRACEFILE;

	m_trace = ss.str();

	std::vector<llvm::StringRef> trace;
	Tokenize(trace, m_trace, ';');

	for (llvm::StringRef& ref : trace)
	{
		//ref = F:main,BB:2

		std::vector<llvm::StringRef> traceElements;
		Tokenize(traceElements, ref, ',');

		llvm::StringRef fName = traceElements.front().substr(2);
		llvm::StringRef bbID = traceElements.back().substr(3);

		if (!fName.empty() && m_traceMap.find(fName) == m_traceMap.end())
			m_traceMap[fName];

		if (!bbID.empty())
			m_traceMap[fName].push_back(bbID);
	}
	
	return NO_ERRORS;
}


int EnergyAnalysis::StartEnergyAnalysis()
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
	if (!m_clockspeed)
		m_clockspeed = 16;

	std::string clockspeed = std::to_string(m_clockspeed) + "Mhz";
	if (m_clockspeed > 1000)
		clockspeed = std::to_string((m_clockspeed / 1000)) + "Ghz";

	log.LogConsole("Clock speed was set to " + clockspeed + "\n\n");


	std::unique_ptr<EnergyModule> energy(new EnergyModule(*Mod));

	// Handles Energy Annotations implemented by the EnergyAnalysis.h header file
	std::unique_ptr<AnnotationVisitor> annotate(new AnnotationVisitor(log.GetLevel()));
	Error = energy->accept(*annotate);
	if (Error)
		return ExitProgram(Error);
	if (log.GetLevel() >= Log::INFO)
		annotate->Print(*Mod);

	// Calculates and stores all cost per instruction in a FunctionMap
	WCETAnalysisVisitor  wcetAnalysis(m_clockspeed);
	Error = energy->accept(wcetAnalysis);
	if (Error)
		return ExitProgram(Error);
	if (log.GetLevel() >= Log::INFO)
		wcetAnalysis.Print();
	
	//Make the final calculation of energy consumption
	EnergyCalculation energyCalculation(m_traceMap, wcetAnalysis, log.GetLevel());
	if (log.GetLevel() >= Log::INFO)
		energyCalculation.PrintTrace();
	Error = energy->accept(energyCalculation);
	if (Error)
		return ExitProgram(Error);
	if (log.GetLevel() >= Log::INFO)
		energyCalculation.Print();

	return Error;
}


bool EnergyAnalysis::IsValidFile(const char * filename)
{

	//check if file is valid
	std::ifstream infile(filename);
	return infile.good();
	
}


bool EnergyAnalysis::IsWriteable(const char* filename)
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