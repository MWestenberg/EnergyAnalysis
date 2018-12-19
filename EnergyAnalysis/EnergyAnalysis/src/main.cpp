#include "Analysis/EnergyAnalysis.h"


int main(int argc, char** argv)
{

	EnergyAnalysis energy;
	int ArgCheck = energy.CheckArguments(argc, argv);

	if (ArgCheck != EnergyAnalysis::NO_ERRORS)
		return EnergyAnalysis::ExitProgram(ArgCheck);

	return energy.StartEnergyAnalysis();
}