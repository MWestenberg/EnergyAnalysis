const int led = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led, OUTPUT);  
  digitalWrite(led, LOW);
  Serial.println("Begin:");
  delay(1000);
}

void bla(char i)
{
  if (i) 
    digitalWrite(led, HIGH);  
  else
    digitalWrite(led,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:


long start = micros();
for (int i=0; i < 1000;i++)
{
   for (int x=0;x < 1000;x++)
   {
       if (x % 2 == 0 && i %2 ==0)
       {
          bla(1);
       }
       else
       {
          bla(0);
       }
   }
}
Serial.println(String((micros() - start)/1000) + " milliseconds");
 

}
