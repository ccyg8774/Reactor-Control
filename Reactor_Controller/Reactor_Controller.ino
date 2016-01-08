#include "Adafruit_MAX31855.h"
#include <LiquidCrystal.h>

int coolingThreshold = 30;  //The first threshold to stop heating and start cooling
int warningThreshold = 35;  //The second threshold to give warning
const int thermoCLK = 26;
const int thermoCS = 24;
const int thermoDO = 22;
const int speakerPin = 12;
const int relayPin = 13; //temperiary assignment
const int detectorPin = A0;
int detectorValue = 0;
int detectorStatus = 0;
volatile int NbTopsFan; //measuring the rising edges of the signal
const int flowPin = 3;
const int valvePin = 28; 
int flowStatus = 0;
const int alarmlightPin = 5;
long previousMillis = 0; 
float temperature = 0;
long intervalbuzzattention = 10000;
long intervalbuzzwarning = 500;
//long detectorDelay = 1000;
//long detectorMillis = 0;
//long flowMillis = 0;
//long flowDelay = 1000;


// Initialize the Thermocouple
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);
     
  void rpm ()
{ 
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
    delay(500);
    }
     
  void loop(){
    unsigned long currentMillis = millis();
    NbTopsFan = 0;      //Set NbTops to 0 ready for calculations
    sei();            //Enables interrupts
    temperature = thermocouple.readCelsius();//read the temperature from the TC
    //lcd.setCursor(0, 0);//Display the temperature on fitsr line of the LCD
    //lcd.print("Temp =    ");
      if (temperature > 2){
        lcd.setCursor(10, 0);
        lcd.print(temperature);
        lcd.setCursor(15, 0);
        lcd.print("C"); 
        delay(500);
          cli();            //Disable interrupts
          detectorValue = analogRead(A0);
          lcd.setCursor(0, 1); //Display the status of the flow and heating.
          if (detectorValue>500){//Change the 500 and 100 value for the light on the actual oven
            lcd.print("Heat ON  ");}
          else if (detectorValue < 100){//Change the 500 and 100 value for the light on the actual oven
            lcd.print("Heat OFF ");}
          lcd.setCursor(9, 1);
          if (NbTopsFan == 0 ){
            lcd.print("Flw OFF");
            flowStatus = 0;}
          else if (NbTopsFan != 0){
            lcd.print("Flw ON "); 
            flowStatus = 1;}
      }
      
      
    if (temperature < coolingThreshold and temperature >= 2){
      lcd.setCursor(0, 0);
      lcd.print("Temp =    ");
      lcd.setCursor(0, 1);
      digitalWrite(alarmlightPin, LOW);
      digitalWrite(relayPin, HIGH);
      digitalWrite(valvePin, LOW);
        if (detectorValue < 100){//in case the heat is not on when it should
          if(currentMillis - previousMillis > intervalbuzzattention){
            previousMillis = currentMillis;
            tone(speakerPin, 3600, 150);}}
    }
      
    else if (temperature >= coolingThreshold and temperature <= warningThreshold){
      lcd.setCursor(0, 0);//Display the temperature on fitsr line of the LCD
      lcd.print("Temp =    ");
      digitalWrite(relayPin, LOW);
      digitalWrite(valvePin, HIGH);
      lcd.setCursor(0, 1);
      if (flowStatus == 1 and detectorValue < 100){
        //if the heat is off and flow is on properly
        digitalWrite(alarmlightPin, LOW);
        if(currentMillis - previousMillis > intervalbuzzattention){
          previousMillis = currentMillis;
          tone(speakerPin, 3600, 150);}}
      else if (flowStatus == 0 and detectorValue < 100){
        //if the flow do not turn on but oven turned off
        digitalWrite(alarmlightPin, HIGH);
        if(currentMillis - previousMillis > intervalbuzzwarning){
          previousMillis = currentMillis;
          tone(speakerPin, 3600, 150);}
        lcd.setCursor(0, 0);
        lcd.print("Flow fail ");}
      else if (detectorValue > 500 and flowStatus != 0){
        //if the heat do not turn off but flow turned on
        digitalWrite(alarmlightPin, HIGH);
        if(currentMillis - previousMillis > intervalbuzzwarning){
          previousMillis = currentMillis;
          tone(speakerPin, 3600, 150);}
        lcd.setCursor(0, 0);
        lcd.print("Oven Err  ");}
      else if (flowStatus == 0 and detectorValue>500){
        //if both the flow and oven are wrong
        digitalWrite(alarmlightPin, HIGH);
        if(currentMillis - previousMillis > intervalbuzzwarning){
          previousMillis = currentMillis;
          tone(speakerPin, 3600, 150);}
        lcd.setCursor(0, 0);
        lcd.print("Total Err ");}
    }
      
      
    else if (temperature > warningThreshold){
      digitalWrite(relayPin, LOW);
      digitalWrite(valvePin, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Overheat  ");
      if(currentMillis - previousMillis > intervalbuzzwarning){
        previousMillis = currentMillis;
        digitalWrite(alarmlightPin, HIGH);
        tone(speakerPin, 3600, 150);}
     }
    else if (temperature < 2){
      digitalWrite(relayPin, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Temp = ");
      lcd.setCursor(7, 0);
      lcd.print("ERROR!  ");
      lcd.setCursor(0, 1);
      lcd.print("Warning:TC Error");
      if(currentMillis - previousMillis > intervalbuzzwarning){
        previousMillis = currentMillis;
        digitalWrite(alarmlightPin, HIGH);
        tone(speakerPin, 3600, 150);}
    }
  }
