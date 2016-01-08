#include "Adafruit_MAX31855.h"
#include <LiquidCrystal.h>

int coolingThreshold = 70;  //The first threshold to stop heating and start cooling
int warningThreshold = 85;  //The second threshold to give warning
const int thermoCLK = 26;
const int thermoCS = 24;
const int thermoDO = 22;
const int speakerPin = 12;
const int relayPin = 14; 
const int detectorPin = A0;
int heatingStatus = 0;
volatile int NbTopsFan; //measuring the rising edges of the signal
const int flowPin = 3;
int flowStatus = 0;
const int valvePin = 28; 
const int alarmlightPin = 5;
unsigned long currentMillis;
long previousMillis = 0; 
float temperature = 0;
long intervalbuzzattention = 20000;
long intervalbuzzwarning = 500;
//long detectorDelay = 1000;
//long detectorMillis = 0;
//long flowMillis = 0;
//long flowDelay = 1000;
// Initialize the Thermocouple
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);
     
void rpm (){ 
  NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 
void setup(){
  pinMode(alarmlightPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(valvePin, OUTPUT);
  pinMode(flowPin, INPUT); 
  attachInterrupt(1, rpm, RISING); //and the interrupt is attached
  pinMode(detectorPin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Initializing");
  digitalWrite(alarmlightPin, HIGH);
  tone(speakerPin, 3600, 300);
  delay(100);
  heatingStatus = readHeating();//reading the detectors
  flowStatus = readFlow();
  //dispFlow(flowStatus);//display the detector status to the LCD
  //dispHeating(heatingStatus);
}//end of setup()
     
void loop(){
  //currentMillis = millis();
  temperature = thermocouple.readCelsius();//read the temperature from the TC
  lcd.setCursor(0, 0);//display the temperature on the LCD
  lcd.print("Temp =    ");
  lcd.setCursor(10, 0);
  lcd.print(temperature);
  lcd.setCursor(15, 0);
  lcd.print("C"); 
  //if (temperature != 0){//testing normal reading from TC
  //}
  if (temperature < coolingThreshold-2 and temperature != 0){
  //temperature below the cooling threshold, need heating.
    normalHeating();
    if (heatingStatus == 0){
      //in case the heat is not on when it should
      ovenOffDuringNormalHeating();
    }
  }//end of heating range code  
  else if (temperature >= coolingThreshold-2 and temperature <= coolingThreshold){
    //do nothing
  }
  else if (temperature > coolingThreshold and temperature <= warningThreshold){
    //Within this range, heating should be off and flow should be on
    digitalWrite(relayPin, LOW);
    digitalWrite(valvePin, HIGH);
    //lcd.setCursor(0, 1);
    if (flowStatus == 1 and heatingStatus == 0){
      //if the heat is off and flow is on properly
      normalCooling();
    }
    else if (flowStatus == 0 and heatingStatus == 0){
      //if the flow do not turn on but oven turned off
      flowFail();
    }
    else if (flowStatus == 1 and heatingStatus == 1){
      //if the heat do not turn off but flow turned on
      heatingError();
    }
    else if (flowStatus == 0 and heatingStatus == 1){
      //if both the flow and oven are wrong
      flowAndHeatingError();
    }
  }
  else if (temperature > warningThreshold){
    temperatureOverWarningThreshold();
  }
  else if (temperature == 0){
    thermocoupleError();
  }
heatingStatus = readHeating();//reading the detectors
flowStatus = readFlow();
dispFlow(flowStatus);//display the detector status to the LCD
dispHeating(heatingStatus);
delay(300);
}
//end of loop()

void thermocoupleError(){
  digitalWrite(relayPin, LOW);
  lcd.setCursor(7, 0);
  lcd.print("ERROR!  ");
  lcd.setCursor(0, 1);
  lcd.print("Warning:TC Error");
  //if(currentMillis - previousMillis > intervalbuzzwarning){
    //previousMillis = currentMillis;
  digitalWrite(alarmlightPin, HIGH);
  beep(intervalbuzzwarning);
  //tone(speakerPin, 3600, 150);}
}
void temperatureOverWarningThreshold(){
  digitalWrite(relayPin, LOW);
  digitalWrite(valvePin, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Overheat  ");
  //if(currentMillis - previousMillis > intervalbuzzwarning){
  //previousMillis = currentMillis;
  digitalWrite(alarmlightPin, HIGH);
  beep(intervalbuzzwarning);
  //tone(speakerPin, 3600, 150);}
}
void flowAndHeatingError(){
  digitalWrite(alarmlightPin, HIGH);
  beep(intervalbuzzwarning);
  lcd.setCursor(0, 0);
  lcd.print("Both Err  ");
}
int readHeating(){
  int detectorValue = 0;
  int detectorRE = 0;
  detectorValue = analogRead(A0);
  if (detectorValue>300){
    //Change the 500 and 100 value for the light on the actual oven
    detectorRE = 1;
  }
  else if (detectorValue < 200){
    //Change the 500 and 100 value for the light on the actual oven
    detectorRE = 0;
  }
  return detectorRE;
}
int readFlow(){
  int flowRE = 0;
  NbTopsFan = 0;      //Set NbTops to 0 ready for calculations
  sei();            //Enables interrupts
  delay(200);
  cli();            //Disable interrupts
  if (NbTopsFan == 0 ){
    flowRE = 0;
  }
  else if (NbTopsFan != 0){
    flowRE = 1;
  }
  return flowRE;
}
void dispFlow(int fs){
  lcd.setCursor(9, 1);
  if (fs == 1){
    lcd.print("Flw ON ");
  }
  else if (fs == 0){
    lcd.print("Flw OFF");
  }
}
void dispHeating(int hs){
  lcd.setCursor(0, 1); //Display
  if (hs == 1){
    lcd.print("Heat ON  ");
  }
  else if (hs == 0){
    lcd.print("Heat OFF ");
  }
}
void beep(int beepInterval){
  currentMillis = millis();
  if(currentMillis - previousMillis > beepInterval){
    previousMillis = currentMillis;
    tone(speakerPin, 3600, 150);
  }
}
void normalHeating(){
  digitalWrite(alarmlightPin, LOW);
  digitalWrite(relayPin, HIGH);
  digitalWrite(valvePin, LOW);
  beep(intervalbuzzattention);
}
void ovenOffDuringNormalHeating(){
  lcd.setCursor(0, 0);
  lcd.print("Oven Err  ");
  beep(intervalbuzzwarning);
}
void normalCooling(){
  digitalWrite(alarmlightPin, LOW);
  beep(intervalbuzzattention);
}
void flowFail(){
  digitalWrite(alarmlightPin, HIGH);
  beep(intervalbuzzwarning);
  lcd.setCursor(0, 0);
  lcd.print("Flow fail ");
}
void heatingError(){
  digitalWrite(alarmlightPin, HIGH);
  beep(intervalbuzzwarning);
  lcd.setCursor(0, 0);
  lcd.print("Oven Err  ");
}
      


