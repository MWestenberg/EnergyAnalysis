#include <stdbool.h>
#include "EnergyAnalysis.h"

enum { LOW, HIGH };


ENERGY(initiate_temp_sensor, bool, "TEMPSENSOR,2,5,1");                  // ec = 100J, t = 2
ENERGY(measure_temp, float, "MEASURETEMP,0,5,1");


bool fan_is_on();
void turn_fan_on();
void turn_fan_off();

bool heating_is_on();
void turn_heating_on();
void turn_heating_off();

void SerialBegin(int);

void Setup
{
	SerialBegin(9600);

}


void Loop()
{


}

void climateControlSystem() {

	initiate_temp_sensor();
	float temp = 0.0;

	temp = measure_temp();
	sleep(200);


	if (temp > 20 && temp < 21) {
		if (fan_is_on()) { //if the fan is still on turn it off now
			turn_fan_off();

			if (heating_is_on()) { // if the heating is on turn it off
				turn_heating_off();

				return; //do nothing
			}

			// heating loop
			while (temp < min_temp) {

				if (fan_is_on()) { //if the fan is still on turn it off now
					turn_fan_off();
					sleep(5000); //wait until it is turned off

				}
				if (!heating_is_on())  // if the heating is not on yet turn it on
					turn_heating_on();

				temp = measure_temp();
				sleep(200);
			}


			//cooling loop
			while (temp > max_temp) {

				// it is now too hot
				if (heating_is_on()) { // if the heating is on turn it off
					turn_heating_off();
					sleep(5000); //wait until the heating is turned off
				}
				if (!fan_is_on()) // if the fan is not on yet turn it on.
					turn_fan_on();

				temp = measure_temp();
				sleep(200);
			}


			sleep(30000); // we wait a minimum of 30 seconds before we continue
			return;

		}

		int main() {

			do {
				climateControlSystem();
			} while (true);


			print("An error occurred.\n");

			return 1;

		}