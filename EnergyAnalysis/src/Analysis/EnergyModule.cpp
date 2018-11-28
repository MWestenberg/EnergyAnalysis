#include "EnergyModule.h"

int EnergyModule::accept(AnalysisVisitor & a)
{
	return a.visit(*this);
}


