#include <stdio.h>
#include <stdlib.h>

int doubleIt(int x) {
	x = x + x;
	return x;
}


int main(int argc, char **argv) {

	if (argc == 2 && argv[1] != NULL) {

		int x = atoi(argv[1]);

		while (x  < 10) {
			x++;
			//x = doubleIt(x);
			printf("%d\n", x);
		}
	}

}