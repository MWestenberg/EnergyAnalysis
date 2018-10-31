#include <stdio.h>

int doSomething(int, int);


int main() {
	
	printf("Hello world");
	printf("Result: %d", doSomething(4,5));


}

int doSomething(int a, int b) {
	
	int c = a * b;
	int d = c * 2;
	return d;
}