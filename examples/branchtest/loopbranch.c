#include <stdio.h>
#include <stdbool.h>
#include "../EnergyAnalysis.h"


ENERGY(start_assembly_line, bool, "AssemblyLine,0,100,2");
bool method1(int i);
bool method2(int x);

bool method1(int i)
{
    printf("m1: %d\n", i);
	if (i > 3)
		return true;
	return false;
};

bool method2(int x) 
{
	printf("m2: %d \n", x);
	if (x > 1)
		return true;
	return false;
};


void loop(int *arr, size_t size) {
	start_assembly_line();
    for (int i = 0; i < size; i++) {
    	LOOP_TRIPCOUNT(5);
		if (arr[i] % 2 == 0) {
			if (method1(arr[i])) {
				printf("branch1");

			}
			else if (method2(arr[i])) {
				printf("branch2");
			}
			else
			{
				printf("No branch");
			}
		}
			
    }
}


int main(int argc, const char * argv[]) {
	
    int a[] = { 1 , 2 , 3 , 4  };
    int s = sizeof a / sizeof *a;
    loop(a, s);
    return 0;
}
