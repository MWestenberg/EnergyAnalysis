#include "Analysis/EnergyAnalysis.h"


int main(int argc, char** argv)
{


	//long double cost;
	//unsigned cycles = 1;
	//unsigned clockspeed = 16;

	//cost = double(1) / double(clockspeed);

	//std::cout << std::fixed;
	//std::cout << std::setprecision(15) << cycles << "/" << clockspeed << "=" << cost << MICROSECONDS << std::endl;

	//return 1;

	EnergyAnalysis energy;
	int ArgCheck = energy.CheckArguments(argc, argv);

	if (ArgCheck != EnergyAnalysis::NO_ERRORS)
		return EnergyAnalysis::ExitProgram(ArgCheck);

	return energy.StartEnergyAnalysis();
}