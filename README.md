# Reactor-Control
This is a control program for the Arduino based controller of the polymerization reactor. 
It measure and display the temperature of the type K thermocoupler inside the reactor. When 
the temperature passed the first threshold, the controller will assume the temperature required
for the polymerization reaction is reached, and turn off the hot plate via a LOW output to the
relay, turn on the cooling water, in order to stop the temperature from overshot. In the case 
that the temparature can not be controlled due to exothermic reaction, and the temperature 
passed the second threshold, the controller will turn on the alarm light, there will be quick 
beep, to notify the operator that immidate action is required. 
The controller also detects the cooling water flow, and warns if it is not on when it should be on. 
