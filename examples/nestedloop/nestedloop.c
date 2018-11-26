#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "EnergyAnalysis.h"

ENERGY(start_assembly_line, bool, "AssemblyLine,0,100,2");

int main() {
	
	int c = 0;

	for (int i=0; i <1000; i++) {
		for (int j=0; j<i ; j++) {
			start_assembly_line();
			c++;
		}
	}	


	printf("result=%d", c);
	return 0;

}