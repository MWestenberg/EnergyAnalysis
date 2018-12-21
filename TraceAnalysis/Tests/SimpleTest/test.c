#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif



int main(int argc, const char * argv[]) {
	printf("Blaat\n");


    return 0;
}
