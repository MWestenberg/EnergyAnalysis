#include <stdio.h>

int main() {
	
	int c = 0;

	for (int i=0; i <10; i++) {
		for (int j=0; j<i ; j++) {
			c++;
		}
	}	


	printf("result=%d", c);
	return 0;

}