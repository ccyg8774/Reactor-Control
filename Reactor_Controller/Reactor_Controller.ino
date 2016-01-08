#include "Adafruit_MAX31855.h"
#include <LiquidCrystal.h>

int coolingThreshold = 30;  //The first threshold to stop heating and start cooling
int warningThreshold = 35;  //The second threshold to give warning
const int thermoCLK = 26;
const int thermoCS = 24;
const int thermoDO = 22;
const int speakerPin = 12;
const int relayPin = 13;
//const int detectorPin = 4;
//const int flowPin = 3;
const int alarmlightPin = 5;
long previousMillis = 0; 
float temperature = 0;
long intervalbuzzattention = 10000;
long intervalbuzzwarning = 500;
//long detectorDelay = 1000;
//long detectorMillis = 0;
//long flowMillis = 0;
//long flowDelay = 5000;


// Initialize the Thermocouple
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);
     
  void setup(){
    pinMode(alarmlightPin, OUTPUT);
    pinMode(relayPin, OUTPUT);
    //pinMode(detectorPin, INPUT);
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.print("Initializing");
    delay(1000);
    }
     
  void loop(){
    unsigned long currentMillis = millis();
    int sensorValue = analogRead(A0);
    temperature = thermocouple.readCelsius();
    lcd.setCursor(0, 0);
    lcd.print("Temp = ");
      if (temperature > 2){
        lcd.setCursor(7, 0);
        lcd.print(temperature);
        lcd.setCursor(12, 0);
        lcd.print(" C"); 
        delay(100);}
    
    if (temperature < coolingThreshold and temperature >= 2){
      lcd.setCursor(0, 1);
      digitalWrite(alarmlightPin, LOW);
      digitalWrite(relayPin, HIGH);
      lcd.print("Heating On      ");
    }
      
    else if (temperature >= coolingThreshold and temperature <= warningThreshold){
      digitalWrite(relayPin, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Attn: Cooling On");
      digitalWrite(alarmlightPin, LOW);
      if(currentMillis - previousMillis > intervalbuzzattention){
        previousMillis = currentMillis;
        tone(speakerPin, 3600, 150);}
    }
      
      
    else if (temperature > warningThreshold){
      digitalWrite(relayPin, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Warning:Overheat");
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
