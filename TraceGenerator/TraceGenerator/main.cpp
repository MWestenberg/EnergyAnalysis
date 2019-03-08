#include "TraceGenerator.h"


int main(int argc, char** argv)
{
	TraceGenerator traceGen;
	int check = traceGen.CheckArguments(argc, argv);

	if (check != TraceGenerator::NO_ERRORS)
		return TraceGenerator::ExitProgram(check);
	   
	return traceGen.StartTraceGenerator();

	
}