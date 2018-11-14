#include <stdbool.h>
#include "EnergyAnalysis.h"


struct MeasureMents
{
    float tlx, tly; //top left position
    float brx, bry; // bottom right position
};


// Simple struct to store the line data needed for the laser
struct CuttingLine
{
    float x1,y1; // bottom position
    float x2,y2; // top position
};




/* External component calls using prototype functions and annotations
    signed int pd (powerdraw): time dependent energy consumption (can be negative to annoate stopping of the powerdraw)
    unsigned int ec (energy consumption): time independent energy consumption or single usage
    unsigned int t (time): the estimated time needed to execute a specific function
    
    Powerdraw (pd) can be negative to indicate a decrease of powerdraw. Normally this should be equal to the initial draw.

*/

ENERGY(start_assembly_line, bool, "AssemblyLine,0,100,2");                  // ec = 100J, t = 2
ENERGY(start_sensor, bool, "PlateSensor,5,5,1");                            // pd = 5J, ec = 5J, t = 1
ENERGY(sensor_read, bool, "PlateSensorRead,0,5,2");                         //  ec = 5J, t = 1
ENERGY(stop_sensor, bool, "PlateSensor,0,5,1");                             // pd stops, ec = 5J, t= 1

ENERGY_ARG(fire_laser, bool, "Laser,1000,2000,180", struct CuttingLine c);  // pd = 1000, ec = 2000, t = 180
ENERGY(laser_firing, bool, "LaserFiring,0,0,1");                            // ec = 5, t = 1 (just checks if the laser is still firing)
ENERGY(stop_laser, bool, "Laser,0,1000,10");                                // pd stops, ec = 1000, t = 10
ENERGY(delay, void, "Delay,0,0,5");                                         // delay of t = 5
ENERGY(measure_plate, struct MeasureMents, "MeasurePlate, 0, 5, 2");               // ec = 5, t = 2


bool checkMeasurements(struct MeasureMents m)
{
    return (sizeof(m.tlx) /(sizeof(float)));
}

struct CuttingLine CalculateLine(struct MeasureMents m)
{
    struct CuttingLine line;
    line.x1 = ( m.tlx + m.brx ) / 2;
    line.x2 = line.x1;
    line.y1 = m.bry;
    line.y2 = m.tly; 
    return line;
}

void startController()
{
    
    bool assembly_line_moved  = start_assembly_line();
    
    if (assembly_line_moved)
    {
        bool sensor_started = start_sensor();
        if (sensor_started)
        {
            bool found_plate = sensor_read();
            bool sensor_stopped = stop_sensor();
            if (found_plate && sensor_stopped)
            {
                struct MeasureMents m = measure_plate();
                
                if (checkMeasurements(m))
                {
                    struct CuttingLine line = CalculateLine(m);
                    bool laser_busy = fire_laser(line);
                
					int x = 0;
                    do 
                    {
                        delay(5);
                        laser_busy = laser_firing();
						
                    } while(laser_busy);
                    stop_laser();
            
                }
            }
            
        }
    }
    
    
    
}


int main(int argc, const char * argv[]) {
	if (argc < 1)
        return 0;

    int  number_of_plates = (int)argv[1];
	do{
		startController();
        number_of_plates--;
        delay(5);
		

	} while (number_of_plates> 0);
	
    return 0;
  
}
