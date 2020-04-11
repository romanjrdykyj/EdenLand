
#include <OneWire.h>
#include <DallasTemperature.h>





//************************* User Defined Variables ********************************************************//


//##################################################################################
//-----------  Do not Replace R1 with a resistor lower than 300 ohms    ------------
//##################################################################################


int R1 = 1000;
int Ra = 25; //Resitance of Digital Pin, 25 ohms for mega/uno
int ECPin = 34;
int ECGround = 35;
int ECPower = 36;


//************************* User Defined Variables ********************************************************//




//*********** Converting to ppm [Learn to use EC it is much better**************//
// Hana      [USA]         PPMconverion:   0.5
// Eutech    [EU]          PPMconversion:  0.64
//Tranchen   [Australia]   PPMconversion:  0.7
// Why didnt anyone standardise this?


float PPMconversion = 0.64;


//*************Compensating for temperature ************************************//
//The value below will change depending on what chemical solution we are measuring
//0.019 is generaly considered the standard for plant nutrients [google "Temperature compensation EC" for more info
float TemperatureCoef = 0.019; //this changes depending on what chemical we are measuring




//********************** Cell Constant For Ec Measurements *********************//
//Mine was around 2.9 with plugs being a standard size they should all be around the same
//But If you get bad readings you can use the calibration script and fluid to get a better estimate for K
float K = 1.76;




//************ Temp Probe Related *********************************************//
#define ONE_WIRE_BUS 4           // Data wire For Temp Probe is plugged into pin 10 on the Arduino





//***************************** END Of Recomended User Inputs *****************************************************************//




//********************************************************//



OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.




float Temperature = 10;
float EC = 0;
float EC25 = 0;
int ppm = 0;


float raw = 0;
float Vin = 5;
float Vdrop = 0;
float Rc = 0;




//*********************************Setup - runs Once and sets pins etc ******************************************************//
void setup()
{
  Serial.begin(9600);

  pinMode(ECPin, INPUT);
  pinMode(ECPower, OUTPUT); //Setting pin for sourcing current
  pinMode(ECGround, OUTPUT); //setting pin for sinking current
  digitalWrite(ECGround, LOW); //We can leave the ground connected permanantly

  delay(100);// gives sensor time to settle
  sensors.begin();
  delay(100);
  //** Adding Digital Pin Resistance to [25 ohm] to the static Resistor *********//
  // Consult Read-Me for Why, or just accept it as true
  R1 = (R1 + Ra);


  //************** Restart Protection Stuff ********************//


  Serial.println("ElCheapo Arduino EC-PPM measurments");
  Serial.println("By: Michael Ratcliffe  Mike@MichaelRatcliffe.com");
  Serial.println("Free software: you can redistribute it and/or modify it under GNU ");
  Serial.println("");
  Serial.println("Make sure Probe and Temp Sensor are in Solution and solution is well mixed");
  Serial.println("");
  Serial.println("Measurments at 5's Second intervals [Dont read Ec morre than once every 5 seconds]:");



  GetEC(); //gets first reading for LCD and then resets max/min
};


//******************************************* End of Setup **********************************************************************//




//************************************* Main Loop - Runs Forever ***************************************************************//
//Moved Heavy Work To subroutines so you can call them from main loop without cluttering the main loop
void loop()
{

  if ((millis() % 10000) <= 100) {
    GetEC();           //Calls Code to Go into GetEC() Loop [Below Main Loop] dont call this more that 1/5 hhz [once every five seconds] or you will polarise the water
  };
  PrintReadings();   // Cals Print routine [below main loop]
  delay(100); //Stops us entering the GETEC loop twice


}
//************************************** End Of Main Loop **********************************************************************//




//************ This Loop Is called From Main Loop************************//
void GetEC() {
  //*********Reading Temperature Of Solution *******************//


  sensors.requestTemperatures();// Send the command to get temperatures
  Temperature = sensors.getTempCByIndex(0); //Stores Value in Variable




  //************Estimates Resistance of Liquid ****************//
  digitalWrite(ECPower, HIGH);
  raw = analogRead(ECPin);
  raw = analogRead(ECPin); // This is not a mistake, First reading will be low beause if charged a capacitor
  digitalWrite(ECPower, LOW);


  //***************** Converts to EC **************************//
  Vdrop = (Vin * raw) / 1024.0;
  Rc = (Vdrop * R1) / (Vin - Vdrop);
  Rc = Rc - Ra;
  EC = 1000 / (Rc * K);
  //*************Compensating For Temperaure********************//
  EC25   =  EC / (1 + TemperatureCoef * (Temperature - 25.0));
  ppm = (EC25) * (PPMconversion * 1000);


  ;
}
//************************** End OF EC Function ***************************//




//***This Loop Is called From Main Loop- Prints to serial usefull info ***//
void PrintReadings() {
  Serial.print(ppm);
  Serial.print(" ppm  ");
  Serial.print(Temperature);
  Serial.println(" *C ");
};
