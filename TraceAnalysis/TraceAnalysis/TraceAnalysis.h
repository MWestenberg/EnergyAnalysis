#pragma once
#include "Analysis.h"
#include "TraceModule.h"
#include "AnnotationTransformer.h"
#include "TransformVisitor.h"

class TraceAnalysis
{
public:

	enum ProgramOptions
	{
		SHOWHELP = 1, RUN_PASS
	};

	static const int NUM_OF_TOKENS = 4;

	enum ErrorCode
	{
		NO_ERRORS = 0,
		E_USAGE,
		E_MESSAGE_MISSING_IRFILE,
		E_MESSAGE_INVALID_PASS,
		E_MESSAGE_ERROR_IRFILE,
		E_MESSAGE_INVALID_ENTRY_POINT,
		E_MESSAGE_INVALID_TOKENS
	};

	TraceAnalysis() : m_arg_counter(0) { log.SetLevel(LOGLEVEL); };

	const struct ErrorMessages
	{
		static constexpr const char* USAGE = "TraceAnalysis <IR FILE>";
		static constexpr const char* MESSAGE_MISSING_IRFILE = "First program argument must be an IR FILE";
		static constexpr const char* MESSAGE_ERROR_IRFILE = "Unable to process your input file.";
		static constexpr const char* MESSAGE_INVALID_ENTRY_POINT = "No entry point found. Requires main function to exist.\nCurrent set Main function: ";
		static constexpr const char* MESSAGE_INVALID_TOKENS = "When using the EnergyAnalysis header file to annotate your code with Energy Functions\n you must have at least 4 tokens. This means a name, temporal energy consumption,\n non-temporal or single powerdraw and a time unit";
	};

	static int ExitProgram(int Error = E_USAGE);
	static int ExitProgram(int Error, const std::string& message);
	static ProgramOptions AssignParam(const char* argument);
	static bool IsValidBitcodeFile(const char* filename);

	int CheckArguments(int argc, char** argv);
	int SetInputFile();
	int StartTraceAnalysis();
	
private:

	Log log;

	int m_arg_counter;
	char** m_argv;
	char* m_executable;
	char* m_inputFile;
};
