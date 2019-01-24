const int relayPin = 13;
char input;

void turnOff() 
{
  digitalWrite(relayPin, LOW);
  Serial.println("Off");
  delay(2000);
}

void turnOn()
{
  digitalWrite(relayPin, HIGH);
  Serial.println("On");
  delay(3000);
}

void setup()
{
  pinMode(relayPin, OUTPUT);
  turnOff();
  Serial.begin(9600);
  Serial.println("Get ready");
  delay(2000);
  Serial.println("Input a number: ");

}

void loop()
{
  
  if (Serial.available())
  {
      input = Serial.read();
      if (input == 97) // a
      {
        turnOn();
      }
      else
      {
        turnOff();
      }       
  }
  
}
