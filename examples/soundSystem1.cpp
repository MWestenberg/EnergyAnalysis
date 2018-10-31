/* EnergyUsage
	unsigned int pd (powerdraw): time dependent energy consumption
	signed int ec (energy consumption): time independent energy consumption or single usage
	signed int t (time): the estimated time needed to execute a specific function
	
	Powerdraw (pd) can be negative to indicate a decrease of powerdraw. Normally this should be equal to the initial draw.

*/
void __EnergyUsage(unsigned int pd, signed int ec, signed int t);


void foo() {}
void soundSystemOn(){}
void soundSystemOff(){}
void playBeep(float Mhz) {}
void sleepABit(int seconds){}

int bar(int*p, int q, int *r) __attribute__((annotate("15,20,0")));

int main() {

	int x = 0;

	__EnergyUsage(5, 20, 0);
	soundSystemOn();

	while (x < 5) {
		__EnergyUsage(0,5,2);
		playBeep(20.0);

		__EnergyUsage(0,0,5); //sleep for 5 seconds
		sleepABit(5);
		
		x++;
	}
	__EnergyUsage(-5, 20, 0);
	soundSystemOff();
	return 1;

}

/*
run 1: SoundSystem t=0, pd = 5, ec = 0
200 Joules

run 2: SoundSystem t=0, pd = 5, ec = 20
260  Joules

run 3: SoundSystem t=5, pd = 5, ec = 20
285  Joules


*/
