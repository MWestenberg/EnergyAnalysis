
const int relayPin = 7;
int d[6] = { 5, 3};
bool done = false;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  Serial.println("Get ready");
  delay(5000);
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

void fibonacci(int n)
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
    Serial.println(next);
  }
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
        delay(5000);
      }
      else
      {
        turnOn();
        fibonacci(25000);
      }

    }
  }
  turnOff();
  done = true;
  Serial.write("Program done\n");
  delay (5000);

}
