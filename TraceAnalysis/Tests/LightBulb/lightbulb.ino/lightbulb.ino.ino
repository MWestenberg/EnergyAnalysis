//Henry's Bench
// HC-SR501 Motion Detector
// Sample Sketch
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int ledPin = 13;  // LED on Pin 13 of Arduino
const int relayPin = 7;
int d[6] = { 5, 3, 10, 38, 22, 40};
bool done = false;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  lcd.begin(16, 2);
  digitalWrite(ledPin, LOW);
  digitalWrite(relayPin, LOW);
  Serial.write("Program Started\n");
  lcd.print("hello, world!");
  
}

void turnOn()
{
   digitalWrite(relayPin, HIGH);
   Serial.write("On\n");
   lcd.clear();
   lcd.print("on");
}

void turnOff()
{
   digitalWrite(relayPin, LOW);
   Serial.write("Off\n");
   lcd.clear();
   lcd.print("off");
}


void loop() {
  lcd.setCursor(0, 1);
  
  if (done)
  {
    return;
  }
    
  size_t n = (sizeof(d) / sizeof(d[0]));

  for (int x = 0; x < n; x++)
  {
    for (int i = 0; i < d[x]; i++)
    {
      int mod = i % 5;
        if (mod < 4 && mod > 0 ) {
         turnOff();
         delay(2000);
        }
        else
        {
          turnOn();
          delay(5000);  
        }
         
    }
    Serial.write("Set done.\n");
  }
  turnOff();
  done = true;
  Serial.write("Program done\n");
  delay (5000);
  
}
