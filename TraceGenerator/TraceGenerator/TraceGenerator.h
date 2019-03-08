#pragma once
#include "Analysis.h"
#include "TraceModule.h"
#include "AnnotationTransformer.h"
#include "TransformVisitor.h"

class TraceGenerator
{
private:
	//Argument types
	enum ArgType
	{
		TRACE, IRFILE, OUTPUTFILE
	};

	enum ProgramExecution
	{
		SHOWHELP = 1, RUN_PASS
	};


	Log log;
	ProgramExecution m_action;
	char** m_argv;
	int m_argc = 0;
	int m_arg_counter;
	char* m_executable;
	char* m_inputFile;
	char* m_traceFile;
	char* m_outputFile;
public:

	

	enum ErrorCode
	{
		NO_ERRORS = 0,
		E_USAGE,
		E_MESSAGE_MISSING_IRFILE,
		E_MESSAGE_INVALID_ARGUMENT,
		E_MESSAGE_INVALID_OUTPUT,
		E_MESSAGE_ERROR_IRFILE,
		E_MESSAGE_INVALID_TOKENS
	};

	TraceGenerator() : m_arg_counter(0), m_executable(""), m_inputFile(""), m_traceFile(""), m_outputFile("") { log.SetLevel(LOGLEVEL); };

	const struct ErrorMessages
	{
		static constexpr char* USAGE = "TraceAnalysis <IR FILE> -o <OUTPUT FILE>\n\n  -h Show this help\n  --help Show this help\n  -o <OUTPUT FILE> Bitcode output file\n  -d Show debug information\n  --debug Show debug information";
		static constexpr char* MESSAGE_MISSING_IRFILE = "First program argument must be a valid IR FILE. Use -h or --help to show options.";
		static constexpr char* MESSAGE_INVALID_ARGUMENT = "Argument given is invalid. Use -h or --help to show options.";
		static constexpr char* MESSAGE_INVALID_OUTPUT = "Cannot write to given outputfile. Please check the target location and your permissions";
		static constexpr char* MESSAGE_ERROR_IRFILE = "Unable to process your IR file. File is either corrupt or is not reachable.";
		static constexpr char* MESSAGE_INVALID_TOKENS = "When using the EnergyAnalysis header file to annotate your code with Energy Functions\n you must have at least 4 tokens. This means a name, temporal energy consumption,\n non-temporal or single powerdraw and a time unit";
	};

	// Extis the program with error code and finds the message for that error code
	static int ExitProgram(int Error = E_USAGE);
	// Extis the program with error code and outputs a message
	static int ExitProgram(int Error, const std::string& message);
	// Check if file exists and valid
	static bool IsValidFile(const char* filename);
	//checks if the file can be created and opened
	static bool IsWriteable(const char* filename);
	//checks all program arguments and calls the SetArgument() function
	int CheckArguments(int argc, char** argv);
	//Starts the analysis. CheckArguments must be called before
	int StartTraceAnalysis();
	
private:
	// This function sets all the arguments. It requires a type ArgType
	int SetArgument(ArgType type, const char* value);
	// Reads all arguments and calls set argument when a valid option is found
	int ReadArgument(const char* argument);


};
