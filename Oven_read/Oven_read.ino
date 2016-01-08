#include <LiquidCrystal.h>

const int detectorPin = A0;
int heatingStatus = 0;
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);
int detectorValue = 0;


void setup() {
  pinMode(detectorPin, INPUT);
  Serial.begin(9600);
  delay(100);
}//end of setup()

void loop() {
  readHeating();
}

void readHeating() {
  detectorValue = analogRead(A0);
  lcd.setCursor(0, 0);//display the temperature on the LCD
  lcd.print(detectorValue);
  delay(500);
}

