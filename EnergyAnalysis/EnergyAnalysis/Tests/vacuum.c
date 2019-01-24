#include "EnergyAnalysis.h"
#define HIGH  1
#define LOW 0

ENERGY_ARG(digitalWriteHIGH, void, "digitalWrite,550,0,0.004534", int, int);
ENERGY_ARG(digitalWriteLOW, void, "digitalWrite,0,0,0.004534", int, int);
ENERGY_ARG(SerialPrintln, void, "Serial.println, 0,0,5.084", int);
ENERGY_ARG(delay, void, "delay, 0,0,5000", int);

//end of precompiled header



const int relayPin = 7;
int d[6] = { 5, 3 };
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
				delay(5000);
			}
			else
			{
				turnOn();
				fibonacci(50000);
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