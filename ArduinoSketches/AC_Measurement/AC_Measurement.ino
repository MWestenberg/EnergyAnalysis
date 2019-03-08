#define acs712 A0
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int getZeroValue();

long lastSample = 0; // last sample
long sampleSum = 0; // total of all samples
int sampleCount = 0; //total number of samples
float joules = 0; // Joules counter
long startTime = 0; // initializer
long endTime = 0; // initializer
long totalTime = 0; // total time init
int firstLoop = 1; // on first loop
int voltage = 230; // voltage from wall
int sensitivity = 100; // current meter sensitivity
float vpc = 5/1024; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("loading...");
  
}

void loop() {
  if (firstLoop) {
    lcd.clear();
    lcd.print("Turn on device");
    firstLoop = 0;
  }
  
  if (millis() > lastSample +1)
  {
    //sampleSum = reading from the acs712 squared;
    sampleSum += sq(analogRead(acs712) - 512);
    
    sampleCount++;
    lastSample = millis();
  }
  
  if (sampleCount == 1000)
  {
   
     double mean = sampleSum / 1000;
     double value = sqrt(mean);     
     
     double mv = value * vpc;
     double amps = mv / sensitivity;
     double watts = amps * voltage;
     
     //Serial.println("AmpRMS: " + String(amps) + " MilliVolts: "+ String(mv));
     lcd.clear();
     lcd.setCursor(0,0);
     
     lcd.print("Amps: ");
     lcd.print(amps);
     if (amps > 0.1)
     {  
        joules += (watts *2.0);
          if (!startTime){
            lcd.clear();
            startTime = millis();
         }
         lcd.setCursor(0,0);
         lcd.print("A:");
         lcd.print(amps);
         lcd.print(" W:");
         lcd.print(watts);
         lcd.setCursor(0,1);
         lcd.print("J:");  
         lcd.print(joules); 
        // Serial.println("AmpRMS: " + String(amps) + " MilliVolts: "+ String(mv) + " " + String(watts) +" Watts" + " Joules: " + String(joules));
     }
     else
     {
        if (startTime)
        {
         endTime = millis();
         totalTime += (endTime - startTime)/1000;
         startTime = 0;
        }

       if (totalTime) {
         // Serial.println("TotalTime: " + String(totalTime) + " Joules: " + String(joules));
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Time: ");
          lcd.print(totalTime);
          lcd.print(" sec");
          lcd.setCursor(0,1);
          lcd.print("Joules: ");
          lcd.print(joules);
       }
  
     }

     sampleSum = 0;
     sampleCount = 0;
  }    
}