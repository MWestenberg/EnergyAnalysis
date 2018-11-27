#pragma once
//EnergyClasses
#include "AnalysisVisitor.h"
#include "AnnotationVisitor.h"
#include "EnergyModule.h"
#include "PathAnalysisVisitor.h"
#include "LoopAnalysisVisitor.h"
#include "WCETAnalysisVisitor.h"
#include "Logging.h"




class EnergyAnalysis: public Analysis
{
public:

	enum ProgramOptions
	{
		SHOWHELP = 1, RUN_PASS
	};

	static const int NUM_OF_TOKENS = 4;

	enum ErrorCode
	{
		NO_ERRORS, E_USAGE, E_MESSAGE_MISSING_IRFILE, E_MESSAGE_INVALID_PASS, E_MESSAGE_ERROR_IRFILE, E_MESSAGE_INVALID_ENTRY_POINT, E_MESSAGE_UNDEFINED_LOOP
	};

	EnergyAnalysis() : m_arg_counter(0), m_executable("") { log.SetLevel(LOGLEVEL); };

	static const struct AnalysisType
	{
		static constexpr char* ANNOTATION = "annotation";
		//static constexpr char* ITERATION = "iteration";
	};

	static const struct ErrorMessages
	{
		static constexpr char* USAGE = "EnergyAnalysis <IR FILE> [-pass=annotation|prototype]";
		static constexpr char* MESSAGE_MISSING_IRFILE = "First program arument must be an IR FILE";
		static constexpr char* MESSAGE_INVALID_PASS = "Pass can only be annotation at the moment";
		static constexpr char* MESSAGE_ERROR_IRFILE = "Unable to process your input file.";
		static constexpr char* MESSAGE_INVALID_ENTRY_POINT = "No entry point found. Requires main function to exist.\nCurrent set Main function: ";
		static constexpr char* MESSAGE_UNDEFINED_LOOP = "Undefined loop was detected.\n\nEither define the number of loop iterations directly add the LOOP_TRIPCOUNT(unsigned) function in the body of the loop.";
	};

	static int ExitProgram(int Error = E_USAGE);
	static int ExitProgram(int Error, const std::string& message);
	static ProgramOptions AssignParam(const char* argument);
	static bool IsValidBitcodeFile(const char* filename);

	int CheckArguments(int argc, char** argv);
	int SetInputFile();
	int StartEnergyAnalysis();
	

private:

	Logging log;

	int m_arg_counter;
	char** m_argv;
	char* m_executable;
	char* m_inputFile;

};

