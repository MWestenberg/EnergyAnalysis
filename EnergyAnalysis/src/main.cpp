#include "EnergyAnalysis/EnergyAnalysis.h"



int main(int argc, char** argv)
{

	EnergyAnalysis energy;
	int check1 = energy.CheckArguments(argc, argv);
	int check2 = energy.SetInputFile();


	if (check1 != EnergyAnalysis::NO_ERRORS)
		return EnergyAnalysis::ExitProgram(check1);
	
	if (check2 != EnergyAnalysis::NO_ERRORS)
		return EnergyAnalysis::ExitProgram(check2);
	


	int result = energy.StartEnergyAnalysis();
	return EnergyAnalysis::ExitProgram(result);
	

}