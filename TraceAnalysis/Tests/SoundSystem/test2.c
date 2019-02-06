#include "EnergyAnalysis.h"
#define HIGH  1
#define LOW 0

// n * (t + s)
// n = # iteration
// t is time of execution time of Soundsystem to turn on or off
// i is independent energy
// stel 100 Watt soundsystem = 100 Joules/sec


#include "settings.h"
//end of precompiled header



int main(int argc, char** argv)
{

	if (argc >= 2)
	{
		int n = 20;
        
		
		for (int i = 0; i < n; i++)
		{
			SoundSystemOn();
			PlayBeepAtHz();
			SoundSystemOff();
			delay();
		}
		
		
	}
	return 1;
}




