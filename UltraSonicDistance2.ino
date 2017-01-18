#include <arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_NeoPixel.h>
#include "Smoother.h"

#define DISTANCE_INPUT 19
#define MAX_DELAY 3000

#define NEO_PIXEL 9
#define NUM_LED 5

#define WARN_DIST 20
#define ERR_DIST 10
#define MAX_DIST 50

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, NEO_PIXEL, NEO_GRB + NEO_KHZ800);
int delayTime = 50;
Smoother s(30);
void setup()
{
	strip.begin();
}


void ledOff(){
	strip.setBrightness(0);
	strip.show();
}

void displayOff(){
	alpha4.begin(0x70);
	for(int i = 0; i < 4; i++){
		alpha4.writeDigitRaw(i, 0x0);
	}
	alpha4.writeDisplay();
}

void setLed(int distance){
	int redVal = 0;
	int blueVal = 0;
	int greenVal = 0;
	if(distance < ERR_DIST){
		redVal = 255;
		greenVal = 0;
		blueVal = 0;
	}else if(distance <= WARN_DIST){
		//will be somewhere between blue and red in color
		//lets map blue it will be somewhere between the error and warning distance
		// then map it to the color inbetween, and make red 255- blue's map to produce
		// a color in between blue and red.
		int blueMap = (int)myMap(distance, ERR_DIST, WARN_DIST, 0, 255);
		redVal = 255 - blueMap;
		blueVal = blueMap;
		greenVal = 0;
	}else{
		//set green now. If it is larger then the maximum distance then only show green at full brightness
		int greenMap = 255;
		if (distance < MAX_DIST){
			greenMap = (int)myMap(distance, WARN_DIST,MAX_DIST, 0,255);
		}
		redVal = 0;
		blueVal = 255 - greenMap;
		greenVal = greenMap;
	}

	for(int i = 0; i < NUM_LED; i++){
		strip.setPixelColor(i, redVal, greenVal, blueVal);
	}
	strip.setBrightness(50);
	strip.show();

}
/**
 * Display distance in inches in alphanumeric display
 */
void displayDistance(float distance){
	alpha4.begin(0x70);
	distance *= 10;
	for(int i = 3; i >= 0; i--){
		char num = '0' + ((int)distance) % 10;
		distance /= 10;
		if(i == 2){
			alpha4.writeDigitAscii(i, num, true);
		}else{
			alpha4.writeDigitAscii(i, num);
		}
	}
	alpha4.writeDisplay();
}

/**
 * the standard map in arduino will not map to 0 so this function will map to 0
 * used in get_distance_reading() for debugging purposes
 * 
 * Used in setLed to determine the color needed for how close the object is to display the correct color
 */
float myMap(float x, float inMin, float inMax, float outMin, float outMax)
{
	return (x - inMin) * (outMax - outMin + 1) / (inMax - inMin + 1) + outMin;
}

float getDistanceReading(){
	float input = analogRead(DISTANCE_INPUT);

	//Sensor reads 9.8mv/in for 5v 6.4mv/in for 3.3v
	/* complicated formula, basically map the analogRead (0-1025) to millivolts (0-5000) range
	 *  to get Vi
	 * then apply the sensor distance formula Vm = (Vcc/512) give mv Vcc = 5v so 9.765625 mV/in
	 * so Distance per inch = Vm/Vi
	 *
	 * I read in 4.82 volts on my regulated usb (computer) so 9.414
	 *
	 * I read in 5.05 volts on a regulated wall outlet so  9.863
	 */
	//uncomment for debugging purposes
	//float mapped = my_map(input, 0, 1025, 0, 5000);
	//Serial.print(mapped);
	//Serial.print("    ");
	//double distance_inches = (mapped/9.24);

	//Simplified map plus distance formula into single divide.
	//Purely an optimization to avoid two divisions
	float distanceInches = input * .5;
	return distanceInches;

}

void loop()
{
	delay(delayTime);
	float dist = 0;
	float distRead = getDistanceReading();
	s.add(distRead);
	//Serial.print(distRead);
	//Serial.print("    ");
	dist = s.getSmoothed();
	//Serial.println(dist);

	if (s.exceedVariance(3)){
		//we havent moved enough throughout all of the sampling
		if (delayTime >= MAX_DELAY){
			//If i am at my maximum delay time turn off neopixels
			//and alphanumeric display to conserve life
			Serial.println("Turning off LED");
			ledOff();
			displayOff();
		}else{
			//increase delay time by 1/4 of a second
			delayTime += 250;
		}
	}else{
		//we moved so lets reset delay time and print updated results
		delayTime = 50;
		setLed(dist);
		displayDistance(dist);
	}
}
