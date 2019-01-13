//
//  main.c
//  LightBulb
//
//  Created by Mark Westenberg on 07/12/2018.
//  Copyright © 2018 Mark Westenberg. All rights reserved.
//
//
//  main.c
//  LightBulb
//
//  Created by Mark Westenberg on 07/12/2018.
//  Copyright © 2018 Mark Westenberg. All rights reserved.
//

#include "EnergyAnalysis.h"

ENERGY(turnOn, void, "Light,25,0,0");
ENERGY(turnOff, void, "Light,0,0,0");

ENERGY(delayOff, void, "delay,0,0,2");
ENERGY(delayOn, void, "delay,0,0,8");


bool done = false;
int d[6] = { 5, 3, 10, 38, 22, 40 };

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
				delayOff();
			}
			else
			{
				turnOn();
				delayOn();
			}

		}
	}
	turnOff();
	done = true;

}

//this will be setup and not used in the trace or energy calculation.
//all instructions until the first external energy consuming component are ignored
int main(int argc, const char * argv[]) {
    
	loop();

}


