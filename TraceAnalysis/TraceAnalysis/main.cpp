#include "TraceAnalysis.h"


int main(int argc, char** argv)
{
	TraceAnalysis energy;
	int check1 = energy.CheckArguments(argc, argv);
	int check2 = energy.SetInputFile();


	if (check1 != TraceAnalysis::NO_ERRORS)
		return TraceAnalysis::ExitProgram(check1);

	if (check2 != TraceAnalysis::NO_ERRORS)
		return TraceAnalysis::ExitProgram(check2);



	return energy.StartTraceAnalysis();

	
}