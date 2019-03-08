#include "TraceModule.h"

int TraceModule::accept(AnalysisVisitor & a)
{
	return a.visit(*this);
}
