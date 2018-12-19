#include "TraceAnalysis.h"


int main(int argc, char** argv)
{
	TraceAnalysis energy;
	int check = energy.CheckArguments(argc, argv);

	if (check != TraceAnalysis::NO_ERRORS)
		return TraceAnalysis::ExitProgram(check);



	return energy.StartTraceAnalysis();

	
}