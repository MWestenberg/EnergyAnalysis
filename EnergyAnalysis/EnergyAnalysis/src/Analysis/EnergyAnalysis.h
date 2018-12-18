#pragma once
//EnergyClasses
#include "AnnotationVisitor.h"
#include "EnergyModule.h"
#include "PathAnalysisVisitor.h"
#include "LoopAnalysisVisitor.h"
#include "WCETAnalysisVisitor.h"
#include "EnergyCalculator.h"

#include "../Logging.h"
#include <sstream> 

class EnergyAnalysis: public Analysis
{
private:

	enum ArgType
	{
		TRACE, IRFILE, OUTPUTFILE
	};

	enum ProgramExecution
	{
		SHOWHELP = 1, RUN_PASS
	};

	Logging log;

	int m_arg_counter;
	char** m_argv;
	int m_argc = 0;
	char* m_executable;
	char* m_inputFile;
	char* m_traceFile;
	char* m_outputFile;

	std::string m_trace;
	std::string m_programTrace;

	
	TraceMap m_traceMap;

	ProgramExecution m_action;

public:


	enum ErrorCode
	{
		NO_ERRORS = 0, 
		E_USAGE, 
		E_MESSAGE_MISSING_IRFILE, 
		E_MESSAGE_INVALID_ARGUMENT, 
		E_MESSAGE_INVALID_OUTPUT,
		E_MESSAGE_ERROR_TRACEFILE, 
		E_MESSAGE_ERROR_IRFILE,
		E_MESSAGE_INVALID_ENTRY_POINT, 
		E_MESSAGE_UNDEFINED_LOOP,
		E_MESSAGE_INVALID_TOKENS
	};

	EnergyAnalysis() : m_arg_counter(0), m_executable("") { log.SetLevel(LOGLEVEL); };

	static const struct AnalysisType
	{
		static constexpr char* ANNOTATION = "annotation";
		//static constexpr char* ITERATION = "iteration";
	};

	static const struct ErrorMessages
	{
		static constexpr char* USAGE = "EnergyAnalysis <IR FILE> -trace=<TRACE FILE>\n\n  -h Show this help\n  --help Show this help\n  -o <OUTPUT FILE> default stdOut";
		static constexpr char* MESSAGE_MISSING_IRFILE = "First program argument must be a valid IR FILE. Use -h or --help to show options.";
		static constexpr char* MESSAGE_INVALID_ARGUMENT = "Argument given is invalid. Use -h or --help to show options.";
		static constexpr char* MESSAGE_INVALID_OUTPUT = "Cannot write to given outputfile. Please check the target location and your permissions";
		static constexpr char* MESSAGE_ERROR_IRFILE = "Unable to process your IR file. File is either corrupt or is not reachable.";
		static constexpr char* MESSAGE_ERROR_TRACEFILE = "A tracefile is required to analyse the program. Run the trace analysis tool first to generate one.";
		static constexpr char* MESSAGE_INVALID_ENTRY_POINT = "No entry point found. Requires main function to exist.\nCurrent set Main function: ";
		static constexpr char* MESSAGE_UNDEFINED_LOOP = "Undefined loop was detected.\n\nEither define the number of loop iterations directly add the LOOP_TRIPCOUNT(unsigned) function in the body of the loop.";
		static constexpr char* MESSAGE_INVALID_TOKENS = "When using the EnergyAnalysis header file to annotate your code with Energy Functions\n you must have at least 4 tokens. This means a name, temporal energy consumption,\n non-temporal or single powerdraw and a time unit";
	};

	static int ExitProgram(int Error = E_USAGE);
	static int ExitProgram(int Error, const std::string& message);
	static bool IsValidFile(const char* filename);
	static bool IsWriteable(const char* filename);

	int CheckArguments(int argc, char** argv);
	int StartEnergyAnalysis();
	

private:
	int ImportTrace();
	int SetArgument(ArgType type, const char* value);
	int ReadArgument(const char* argument);

};

