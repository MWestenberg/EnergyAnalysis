#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int func1(int a) {
	printf("a=%d", a);
	return 2;
}


bool func2(int a) {
	if (a > 2) {
		return true;
	}
	else
	{
		return false;
	}

}


int func3(int a) {
	int x = 0;
	while (a < 2) {
		x += a;
	}
	return x;
}


int main( int argc, const char* argv[] )  {
	
	int a = atoi(argv[1]);
	int r = 0;
	if (func2(a)) {
		r = func1(a);
	}
	else
	{
		r = func3(a);
	}

	return r;


}