#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "EnergyAnalysis.h"


int doubleIt(int x) {
	x = x + x;
	return x;
}

ENERGY(start_assembly_line, bool, "AssemblyLine,0,100,2"); 

int main(int argc, char **argv) {

	if (argc == 2 && argv[1] != NULL) {

		int x = atoi(argv[1]);

		while (x  < 10) {
			start_assembly_line();
			x++;
			//x = doubleIt(x);
			printf("%d\n", x);
		}
	}

}