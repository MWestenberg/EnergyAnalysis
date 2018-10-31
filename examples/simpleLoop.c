#include <stdio.h>

/* EnergyUsage
	signed int pd (powerdraw): time dependent energy consumption
	unsigned int ec (energy consumption): time independent energy consumption or single usage
	unsigned int t (time): the estimated time needed to execute a specific function
	
	Powerdraw (pd) can be negative to indicate a decrease of powerdraw. Normally this should be equal to the initial draw.

*/
void __EnergyUsage(signed int pd, unsigned int ec, unsigned int t);

void radioOn(){};
void radioOff(){};

int main() {

	int x = 0;
	__EnergyUsage(5, 0, 0);
	radioOn();

	while (x < 10) {
		__EnergyUsage(0,5,2);
		printf("%d\n",x);
		__EnergyUsage(0,5,2);
		x++;
	}
	__EnergyUsage(-5, 0, 0);
	radioOff();
	return 1;

}



/* Calulations of energy usage:

__EnergyUsage(5, 20, 5);
	radioOn();
__EnergyUsage(0,5,1);
		printf("%d\n",x);
__EnergyUsage(-5, 5, 5);
		radioOff();

x < 4: 135 Joules 
x < 10: 255 Joules

============================
__EnergyUsage(5, 20, 1);
	radioOn();
__EnergyUsage(0,5,1);
		printf("%d\n",x);
__EnergyUsage(-5, 5, 1);
		radioOff();

x < 10:  235 Joules
============================
__EnergyUsage(5, 0, 1);
	radioOn();
__EnergyUsage(0,5,1);
		printf("%d\n",x);
__EnergyUsage(-5, 0, 1);
		radioOff();

x < 10:  205 Joules

============================
__EnergyUsage(5, 0, 0);
	radioOn();
__EnergyUsage(0,5,1);
		printf("%d\n",x);
__EnergyUsage(-5, 0, 0);
		radioOff();

x < 10:  100 Joules

*/
