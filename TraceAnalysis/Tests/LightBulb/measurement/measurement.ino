#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int sensorIn = A0;
int mVperAmp = 100; // use 100 for 20A Module and 66 for 30A Module

int ACVoltage = 230;

float vpc = 4.882812;
 
double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;
double Joules = 0;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.print("loading...");
}

void printJoules()
{


  lcd.setCursor(0,0);
  lcd.print("Wattage: ");
  lcd.print(Joules);
}

void printAmps(double a, double v)
{
  lcd.setCursor(0,1);
  lcd.print("A: ");
  lcd.print(a);
  lcd.print(" V: ");
  lcd.print(v);
}

 
void loop(){

 Voltage = getVPP();
 VRMS = (Voltage/2.0) *0.707;  //root 2 is 0.707
 AmpsRMS = (VRMS * 1000)/mVperAmp;

 Serial.println("AmpRMS: " + String(AmpsRMS) + " MilliVolts: "+ String(VRMS) + " Wattage " + String(AmpsRMS * ACVoltage));
 
 //if ((VRMS * AmpsRMS) > 0.1) 
// {
//  Joules = (VRMS * AmpsRMS) * ACVoltage;
// }


// printJoules();
// printAmps(AmpsRMS, VRMS);
 //Serial.print(AmpsRMS);
// Serial.println(" Amps RMS");

}
 
float getVPP()
{
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       //Serial.println(readValue);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the minimum sensor value*/
           minValue = readValue;

       }
   }

   
   // Subtract min from max
   result = ((maxValue - minValue) * vpc)/1024.0;
      
   return result;
 }
