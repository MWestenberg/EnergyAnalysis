#define acs712 A0
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int getZeroValue();

long lastSample = 0;
long sampleMax = 0;
int sampleCount = 0;

float vpc = 5/1024;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);/*
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("loading...");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);*/
}

void loop() {
  
  
  if (millis() > lastSample +1)
  {
    int currentSample = analogRead(acs712) - 512;
    if (currentSample > sampleMax)
    {
      sampleMax = currentSample;
    }
    sampleCount++;
    lastSample = millis();
  }
  
  
  if (sampleCount == 1000)
  {
   
     float peakMv = sampleMax * vpc;
     float avgMv = (peakMv/2.0) * 0.707; // VRMS
     float amperage = avgMv / 100;
     float wattage = amperage * 230;
     
     Serial.println("AmpRMS: " + String(amperage) + " Approx Wattage: " + String(wattage));
     /*
     lcd.setCursor(0,0);
     lcd.print("A: ");
     lcd.print((mv/100));
     lcd.print(" V: ");
     lcd.print(mv);
     lcd.setCursor(0,1);
     lcd.print("Watts: ");
     lcd.print(watts);
      */
     //Serial.println("Watts " + String(w));
     
    
     sampleCount = 0;
  }
    
}
