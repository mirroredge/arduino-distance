

// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _UltraSonicDistance2_H_
#define _UltraSonicDistance2_H_
#include "Arduino.h"

//end of add your includes here


//add your function definitions for the project WeatherShieldTest here
void setLed(int);
void ledOff();
float myMap(float, float, float, float, float);
float getDistanceReading();
void displayDistance(float);




//Do not add code below this line
#endif /* _UltraSonicDistance2_H_ */
