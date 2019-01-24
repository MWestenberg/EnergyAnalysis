#include "EnergyAnalysis.h"
#define HIGH  1
#define LOW 0
#define led 2

ENERGY_ARG(digitalWriteHIGH, void, "digitalWriteHIGH,0,0,0.004534", int, int);
ENERGY_ARG(digitalWriteLOW, void, "digitalWriteLOW,0,0,0.004534", int, int);
ENERGY(compOn, void, "comp,75,0,0");
ENERGY(compOff, void, "comp,0,0,0");


void bla(bool i)
{
	if (i)
		digitalWriteHIGH(led, HIGH);
	else
		digitalWriteLOW(led, LOW);
}


int main()
{


	compOn();
	for (int i=0; i < 1000;i++)
	{
	   for (int x=0;x < 1000;x++)
	   {
	       if (x % 2 == 0 && i %2 ==0)
	       {
	          bla(true);
	       }
	       else
	       {
	          bla(false);
	       }
	   }
	}
	compOff();
	return 1;
}
