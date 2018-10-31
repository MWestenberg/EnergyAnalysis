#include <stdio.h>

void initiateRobot();
void turnOffRobot();

int main( int argc, char *argv[] )  {


	if( argc != 2 ) {
		return -1;
	}


	unsigned int x= (int)*argv[1]; // %8 = sext i8 %7 to i32
	signed int y=0, a=0, z=0, b=0;

	

	if (x>5){ // %9 = icmp ugt i8 %7, 5
		y=2; //%10 = select i1 %9, i32 2, i32 -2
		a=3; // %11 = select i1 %9, i32 3, i32 0
		z=a+y ; // %12 = select i1 %9, i32 5, i32 0
	}
	else {
		y = -2; // %10 = select i1 %9, i32 2, i32 -2
		// a=0; %11 = select i1 %9, i32 3, i32 0
		// z= 0 %12 = select i1 %9, i32 5, i32 0
	}
	
	if (x>0) { // %13 = icmp eq i8 %7, 0
		// %14 = select i1 %13, i32 -1, i32 1
		z++; // %15 = add nsw i32 %12, %14
		
	}
	else {
		initiateRobot(); // td = 5, ie = 10 t = 0;
		// %14 = select i1 %13, i32 -1, i32 1
		z--; // %15 = add nsw i32 %12, %14
		b=4; // %16 = select i1 %13, i32 4, i32 0
		turnOffRobot();

	}	

	//turn of robot


	printf("%d, %d, %d, %d, %d", x, y, a, z, b); // %17 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str, i64 0, i64 0), i32 %8, i32 %10, i32 %11, i32 %15, i32 %16)

	return 0;

}