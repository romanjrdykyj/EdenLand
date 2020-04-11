int sensor_pin = 14;
int value ;
 
void setup()
{
  Serial.begin(9600);
  Serial.println("Reading");
  delay(2000);
}
 
void loop()
{
 
  value= analogRead(sensor_pin);
  //value=value/4;
  value = map(value,0,4000,0,100);
  Serial.print("Moisture : ");
  Serial.print(value);
  Serial.println("%");
  delay(1000);
}
