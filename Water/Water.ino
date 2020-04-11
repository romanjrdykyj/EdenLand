
#define WATER_SENSOR 2
int stan;

void setup()
{
  Serial.begin (9600);
  pinMode(WATER_SENSOR, INPUT);
}
void loop()
{
  stan = digitalRead(WATER_SENSOR);
  if(stan==1)
  {
    Serial.println("WODA POWYZEJ 20%");
  }
  else
  {
    Serial.println("WODA PONIZEJ 20% - UZUPELNIJ ZBIORNIK!");
  }
  delay(500);
}
