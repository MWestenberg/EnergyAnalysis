const int led = 13;
const int relayPin = 7;
int d[7] = { 5, 3, 7, 10, 120, 51, 38};
bool done = false;
long timer = 0; 

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.println("Get ready");
  digitalWrite(led, HIGH);
  delay(5000);
  digitalWrite(led, LOW);
  Serial.write("Program started\n");
  
}

void turnOn()
{
  digitalWrite(relayPin, HIGH);
}

void turnOff()
{
  digitalWrite(relayPin, LOW); 
}

int fibonacci(int n)
{
  int first = 0, second = 1, next;
  for (int c = 0; c < n; c++)
  {
    if (c <= 1)
      next = c;
    else
    {
      next = first + second;
      first = second;
      second = next;
    }  
    //Serial.println(next);
  }
  delay(20000);
  return next;
}

void loop() {

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
      if (mod < 4 && mod > 0) {
        turnOff();
        delay(4000);
      }
      else
      {
        turnOn();
        int x = fibonacci(1000);     
      }

    }
  }
  turnOff();
  done = true;
  Serial.write("Program done\n");
  digitalWrite(led, HIGH);
  Serial.println("Program took: " + String(timer /1000) + " seconds");
 

}