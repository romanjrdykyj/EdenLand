#define PIN_RESISTANCE          6      // Pin pomiarowy
#define R1                      10000.0 // Rezystancja R1
#define U                       5.0     // Napiecie U




void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Wiglotnosc powietrza: "+ getResistanceString() );
}


String  getResistanceString() {
  int measurementCount = 10;
  double r2Sum = 0.0;
  for(int x = 0; x < measurementCount; x++) {
    double u1 = U * (analogRead(PIN_RESISTANCE)/1023.0);
    r2Sum += (U*R1/u1) - R1;
  }
  double r2 = r2Sum / measurementCount;
  return String(r2);
};
