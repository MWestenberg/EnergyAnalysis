#include <stdio.h>

/* EnergyUsage
	unsigned int pd (powerdraw): time dependent energy consumption
	signed int ec (energy consumption): time independent energy consumption or single usage
	signed int t (time): the estimated time needed to execute a specific function
	
	Powerdraw (pd) can be negative to indicate a decrease of powerdraw. Normally this should be equal to the initial draw.

*/
void __EnergyUsage(unsigned int pd, signed int ec, signed int t);

void radioOn(){};
void radioOff(){};

int main() {

	int x = 0;

	

	while (x < 10) {
		__EnergyUsage(5, 0, 0);
		radioOn();
		__EnergyUsage(0,5,1);
		printf("%d\n",x);
		__EnergyUsage(-5, 0, 0);
		radioOff();
		x++;
	}
	
	return 1;

}

/* Calulations of energy usage:

__EnergyUsage(5, 20, 5);
	radioOn();
__EnergyUsage(0,5,1);
		printf("%d\n",x);
__EnergyUsage(-5, 5, 5);
		radioOff();

x < 4: 285 Joules 
x < 10: 705 Joules

============================
__EnergyUsage(5, 20, 1);
	radioOn();
__EnergyUsage(0,5,1);
		printf("%d\n",x);
__EnergyUsage(-5, 5, 1);
		radioOff();

x < 10: 505 Joules

============================
__EnergyUsage(5, 0, 1);
	radioOn();
__EnergyUsage(0,5,1);
		printf("%d\n",x);
__EnergyUsage(-5, 0, 1);
		radioOff();

x < 10: 250 Joules
============================
__EnergyUsage(5, 0, 0);
	radioOn();
__EnergyUsage(0,5,1);
		printf("%d\n",x);
__EnergyUsage(-5, 0, 0);
		radioOff();

x < 10: 200 Joules

*/
