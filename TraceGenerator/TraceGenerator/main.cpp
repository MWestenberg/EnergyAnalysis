#include "TraceGenerator.h"


int main(int argc, char** argv)
{
	TraceGenerator energy;
	int check = energy.CheckArguments(argc, argv);

	if (check != TraceGenerator::NO_ERRORS)
		return TraceGenerator::ExitProgram(check);
	   
	return energy.StartTraceAnalysis();

	
}