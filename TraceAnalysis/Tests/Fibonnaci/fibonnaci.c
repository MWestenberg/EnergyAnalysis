#include "EnergyAnalysis.h"
#define HIGH  1
#define LOW 0

ENERGY_ARG(digitalWriteHIGH, void, "VacuumCleaner,590,0,0.004534", int, int);
ENERGY_ARG(digitalWriteLOW, void, "VacuumCleaner,0,0,0.004534", int, int);
ENERGY_ARG(SerialPrintln, void, "Serial.println, 0,0,0.785", int);
ENERGY_ARG(delay20, void, "delay20, 0,0,20000", int);
ENERGY_ARG(delay8, void, "delay8, 0,0,8000", int);
//end of precompiled header



const int relayPin = 7;
int d[7] = { 5, 3, 7, 10, 120, 51, 38};
bool done = false;

void turnOn()
{
	digitalWriteHIGH(relayPin, HIGH);
}

void turnOff()
{
	digitalWriteLOW(relayPin, LOW);

}

int fibonacci(int n)
{
	int first = 0, second = 1, next;
	for (int c = 0; c < n; c++)
	{
		if (c <= 1)
			next = c;
		else
		{
			next = first + second;
			first = second;
			second = next;
		}
		//SerialPrintln(next);
		
	}
	delay20(20000);
	return next;
}

void loop() {

	if (done)
	{
		return;
	}

	size_t n = (sizeof(d) / sizeof(d[0]));

	for (int x = 0; x < n; x++)
	{
		for (int i = 0; i < d[x]; i++)
		{
			int mod = i % 5;
			if (mod < 4 && mod > 0) {
				turnOff();
				delay8(4000);
			}
			else
			{
				turnOn();
				int x =fibonacci(1000);
			}

		}
	}
	turnOff();
	done = true;
}

int main()
{
	loop();
	return 0;

}