#include "EnergyModule.h"

void EnergyModule::accept(AnalysisVisitor & a)
{
	a.visit(*this);
}


