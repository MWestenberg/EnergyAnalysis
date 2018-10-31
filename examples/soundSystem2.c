s /* EnergyUsage
	unsigned int pd (powerdraw): time dependent energy consumption
	signed int ec (energy consumption): time independent energy consumption or single usage
	signed int t (time): the estimated time needed to execute a specific function
	
	Powerdraw (pd) can be negative to indicate a decrease of powerdraw. Normally this should be equal to the initial draw.

*/
void __EnergyUsage(unsigned int pd, signed int ec, signed int t);


void soundSystemOn(){};
void soundSystemOff(){};
void playBeep(float Mhz) {};
void sleepABit(int seconds){}
 

int main() {

	int x = 0;

	while (x < 5) {

		__EnergyUsage(5, 20, 0);
		soundSystemOn();

		__EnergyUsage(0,5,2);
		playBeep(20.0);

		__EnergyUsage(-5, 20, 0);
		soundSystemOff();

		__EnergyUsage(0,0,5); //sleep for 5 seconds consumes no energy
		sleepABit(5);
		x++;
	}
	
	
	return 1;

}
/*
run 1: SoundSystem t=0, pd = 5, ec = 0
75 Joules

run 2: SoundSystem t=0, pd = 5, ec = 20
275  Joules

run 3: SoundSystem t=0, pd = 5, ec = 20
400  Joules


*/
