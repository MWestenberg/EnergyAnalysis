const int relayPin = 13;
char input;

bool IsPrime(int n)
{
  bool aPrime = true;
  for (int t = 2; t <= n; t++)
  {
    if (n % t == 0)
    {
      aPrime = false;
      break;
    }
  }
  return aPrime;
}

void turnOff() 
{
  digitalWrite(relayPin, LOW);
}

void turnOn()
{
  digitalWrite(relayPin, HIGH);
}

void setup()
{
  pinMode(relayPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Get ready");
  delay(2000);
  
}

void loop()
{

  int tot = 0;
  long start = millis();
  input = Serial.read();
  int max = 100000;
  turnOn();
  for (int i = 1; i < max; i++)
  {
    if (IsPrime(i))
      tot++;
  }
  turnOff();
  Serial.println("It took :" + String((millis() - start)) + " milliseconds and found " +  String(tot) + " Prime numbers");

      
}
