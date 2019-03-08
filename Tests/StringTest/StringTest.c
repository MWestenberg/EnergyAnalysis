#include "../include/EnergyAnalysis.h"
#define HIGH  1
#define LOW 0

ENERGY_ARG(digitalWriteHIGH, void, "LightBulb,71,0,0.004534", int, int);
ENERGY_ARG(digitalWriteLOW, void, "LightBulb,0,0,0.004534", int, int);
ENERGY_ARG(SerialPrintln, void, "Serial.println, 0,0,0.500", int);
ENERGY_ARG(delayOff, void, "delayOff, 0,0,2000", int);
ENERGY_ARG(delayOn, void, "delayOn, 0,0,3000", int);
//end of precompiled header
const int relayPin = 7;

void turnOff() 
{
  digitalWriteLOW(relayPin, LOW);
  delayOff(2000);
//	printf("Off");
}

void turnOn()
{
 digitalWriteHIGH(relayPin, HIGH);
 delayOn(3000);
 //	printf("On");
 
}



void loop(char* input, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (input[i] == 97)
			turnOn();
		else
			turnOff();
	}

}


int main(int argc, char** argv)
{

	if (argc >= 2)
	{
		char** pargv = argv+1;
		int i =0;
        for (char *ptr = *pargv; *ptr != '\0'; ptr++)
               i++;
        
        loop(argv[1], i);
		
	}
	return 1;
}




