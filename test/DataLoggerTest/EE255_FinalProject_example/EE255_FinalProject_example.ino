void setup()
{
  Serial.begin(38400);
}

int val;

void loop()
{
  val = analogRead(0);
  Serial.println(val);
  delay(250);
}

