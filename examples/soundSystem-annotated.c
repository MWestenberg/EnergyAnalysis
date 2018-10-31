/* EnergyUsage
	signed int pd (powerdraw): time dependent energy consumption
	unsigned int ec (energy consumption): time independent energy consumption or single usage
	unsigned int t (time): the estimated time needed to execute a specific function
	
	Powerdraw (pd) can be negative to indicate a decrease of powerdraw. Normally this should be equal to the initial draw.

*/
		
void soundSystemOn() __attribute__((annotate("5,20,0")));
void soundSystemOff() __attribute__((annotate("-5,20,0")));
void playBeep(float Mhz) __attribute__((annotate("0,5,2")));
void sleepABit(int seconds) __attribute__((annotate("0,0,5")));


void soundSystemOn() {};
void soundSystemOff() {};
void playBeep(float Mhz) {};
void sleepABit(int seconds){};


int main() {

	int x = 0; 

	soundSystemOn();

	while (x < 5) {
		playBeep(20.0);
		sleepABit(5);
		x++;
	}
	soundSystemOff();
	return 1;

}
