#include "../include/EnergyAnalysis.h"
#define HIGH  1
#define LOW 0

ENERGY_ARG(digitalWriteHIGH, void, "VacuumCleaner,75,0,0.004534", int, int);
ENERGY_ARG(digitalWriteLOW, void, "VacuumCleaner,0,0,0.004534", int, int);
ENERGY_ARG(SerialPrintln, void, "Serial.println, 0,0,0.500", int);
ENERGY_ARG(delay5, void, "delay5, 0,0,5000", int);
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
		SerialPrintln(next);
		
	}
	delay5(5000);
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
			if (mod > 0 && mod < 4) {
				turnOff();
				delay5(5000);
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