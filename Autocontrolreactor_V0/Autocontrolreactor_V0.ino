/* Reactor Mornitor and Control
 
 Measure and display the temperature of the type K thermocoupler inside 
 the reactor. 
 When the temperature passed the first threshold, turn off the heating
 plate via a LOW output to the relay, turn on the water and  a red led,
 and beep every 10 seconds.
 When the temperature passed the second threshold, turn on the alarm
 light, beep every second.
 
 The circuit:
 * Red LED attached from pin 13 to ground.
 * Alarm light relay attached from pin 12 to ground.
 * Thermocouple Amplifier board attached from pin 11, 10 and 9. 
 * LCD attached from pin 8 - 3. 
 * Active buzz attached from pin 2
 * Heating stove relay attached from pin A0
 * Solenoid valve relay attached from pin A2

 
 created 2013
 by Yanggang Chen
 
 */

// constants won't change. Used here to
// set pin numbers:
const int redled =  13;      // the number of the red LED pin
const int alarmrelay =  12; 
const int TCampclock =  11; 
const int TCampdataout =  10; 
const int TCampchipselect =  9; 
const int LCDRS =  8; 
const int LCDEN =  7; 
const int LCDDB4 =  6; 
const int LCDDB5 =  5; 
const int LCDDB6 =  4; 
const int LCDDB7 =  3; 
const int activebuzzer =  2; 
const int stoverelay = A0;
const int valverelay = A2;

// Variables will change:
int redledstate = LOW; // redledState used to set the red LED
int alarmstate = LOW;  //used to set the alarm storbe light
float temperature; //variable for the temperature reading
int buzzerstate = LOW;
int stovestate = HIGH; //Stove off when stovestate is HIGH
int valvestate = HIGH; //Valve off when stovestate is HIGH
long previousMillis = 0;        // will store last time buzzer was updated
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)

void setup() {
  // set the digital pin as output:
  pinMode(redled, OUTPUT);
  pinMode(alarmrelay, OUTPUT);

  
}

void loop()
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;  

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

