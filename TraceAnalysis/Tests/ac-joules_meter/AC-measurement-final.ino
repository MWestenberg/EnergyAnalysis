#define acs712 A0
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int getZeroValue();

long lastSample = 0;
long sampleSum = 0;
int sampleCount = 0;
float joules = 0;
long startTime = 0;
long endTime = 0;
long totalTime = 0;


float vpc = 4.882812;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("loading...");
  
}

int firstLoop = 1;

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
     double amps = mv / 100;
     double watts = amps * 230;
     
     Serial.println("AmpRMS: " + String(amps) + " MilliVolts: "+ String(mv));
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
         Serial.println("AmpRMS: " + String(amps) + " MilliVolts: "+ String(mv) + " " + String(watts) +" Watts" + " Joules: " + String(joules));
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
          Serial.println("TotalTime: " + String(totalTime) + " Joules: " + String(joules));
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
