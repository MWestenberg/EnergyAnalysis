#define acs712 A0
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


int getZeroValue();
// 100 millivolts per amp
int sensitivity = 100;
int zeroVal = 0;

float vpc = 4.882812;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("loading...");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("Connect device");
  delay(5000);
  getZeroValue();
}

void loop() {
  double RMSAmp = getVPP() / sensitivity;
  Serial.println(RMSAmp * 230);
  lcd.setCursor(0,0);
  lcd.print("Watts: ");
  lcd.print(RMSAmp * 230);
  
  // put your main code here, to run repeatedly:
  //Serial.println(analogRead(acs712) - zeroVal);
  //Serial.println(analogRead(acs712));

  
  //float value = getVPP();
  //Serial.println(value);
    
}

//First we must get the Average reading of zero value.
//After connecting a device this can fluctuate.
int getZeroValue()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Analyzing...");
  long sample = 0;
  long minValue = 0;
  long maxValue = 0;
  uint32_t start_time = millis();


  //analyze for 5 seconds
  while (millis() - start_time < 5000)
  {
   
      sample = analogRead(acs712);
      if (sample > maxValue)
        maxValue = sample;
      
      if (sample < minValue);
        minValue = sample;
  }

  zeroVal = (maxValue+minValue)/2;
  lcd.print("done");
  delay(2000);
  lcd.clear();
  lcd.print("Average: ");;
  lcd.print(zeroVal);
  delay(2000);
  lcd.clear();
  
}

//returns the voltage in MilliVolts
float getVPP()
{
  int sampleCount = 0;
  long sampleSum = 0;
  long lastSample = 0;
  uint32_t start_time = millis();

  //analyze for 5 seconds
  while (millis() - start_time <= 1000)
  {
    //sampleSum = reading from the acs712 squared;
    sampleSum += sq(analogRead(acs712) - zeroVal);
  }

  double mean = sampleSum / 1000;
  return (sqrt(mean) * vpc);
  
}
