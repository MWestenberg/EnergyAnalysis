#pragma once
//EnergyClasses
#include "AnnotationVisitor.h"
#include "EnergyModule.h"
#include "WCETAnalysisVisitor.h"
#include "EnergyCalculation.h"

#include "../log.h"
#include <sstream> 

class EnergyAnalysis: public Analysis
{
private:

	//Argument types
	enum ArgType
	{
		TRACE, IRFILE, OUTPUTFILE, CLOCKSPEED
	};

	//Program options
	enum ProgramExecution
	{
		SHOWHELP = 1, RUN_PASS
	};

	//member classes
	Log log;
	TraceMap m_traceMap;
	ProgramExecution m_action;

	// member variables
	char** m_argv;
	int m_argc = 0;
	int m_arg_counter;
	char* m_executable;
	char* m_inputFile;
	char* m_traceFile;
	char* m_speedtestFile;
	char* m_outputFile;
	int m_clockspeed = DEFAULT_SPEED;
	std::string m_trace;
	std::string m_programTrace;


public:

	//Possible error codes. These codes each have a corresponding error message
	// except for NO_ERRORS
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
		E_MESSAGE_INVALID_TOKENS,
		E_MESSAGE_ERROR_PARSE_TRACEFILE,
		E_MESSAGE_INVALID_CLOCKSPEED
	};

	//Constructor
	EnergyAnalysis() : m_arg_counter(0), m_executable(""), m_inputFile(""), m_traceFile(""), m_outputFile("") { log.SetLevel(LOGLEVEL); };


	// static error messages used when the program should exit
	static const struct ErrorMessages
	{
		static constexpr char* USAGE = "EnergyAnalysis <IR FILE> -trace=<TRACE FILE>\n\n  -h Show this help\n  --help Show this help\n  -o <OUTPUT FILE> default stdOut\n  -clockspeed=<CLOCKSPEED> Clockspeed in Mhz, default is 16Mhz\n  -speedtest=<ARDUINO SPEEDTESTFILE>\n  -d Show debug information\n  --debug Show debug information\n  -i Show execution information\n  --debug Show execution information";
		static constexpr char* MESSAGE_MISSING_IRFILE = "First program argument must be a valid IR FILE. Use -h or --help to show options.";
		static constexpr char* MESSAGE_INVALID_ARGUMENT = "Argument given is invalid. Use -h or --help to show options.";
		static constexpr char* MESSAGE_INVALID_OUTPUT = "Cannot write to given outputfile. Please check the target location and your permissions";
		static constexpr char* MESSAGE_ERROR_IRFILE = "Unable to process your IR file. File is either corrupt or is not reachable.";
		static constexpr char* MESSAGE_ERROR_TRACEFILE = "A tracefile is required to analyse the program. Run the trace analysis tool first to generate one.";
		static constexpr char* MESSAGE_ERROR_PARSE_TRACEFILE = "Unable to properly parse the tracefile. The source LLVM IR file does not contain some functions or basic blocks defined in the given trace file.";
		static constexpr char* MESSAGE_UNDEFINED_LOOP = "Undefined loop was detected.\n\nEither define the number of loop iterations directly add the LOOP_TRIPCOUNT(unsigned) function in the body of the loop.";
		static constexpr char* MESSAGE_INVALID_TOKENS = "When using the EnergyAnalysis header file to annotate your code with Energy Functions\n you must have at least 4 tokens. This means a name, temporal energy consumption,\n non-temporal or single powerdraw and a time unit";
		static constexpr char* MESSAGE_INVALID_CLOCKSPEED = "The clockspeed must be set in Mhz and should be an integer number greater than 0.";
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
	int StartEnergyAnalysis();
	

private:
	// Imports the trace. Is called from SetArgument
	int ImportTrace();
	// This function sets all the arguments. It requires a type ArgType
	int SetArgument(ArgType type, const char* value);
	// Reads all arguments and calls set argument when a valid option is found
	int ReadArgument(const char* argument);

};

