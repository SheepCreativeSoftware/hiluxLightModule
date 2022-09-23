#line 1 "/home/magraina/projects/hiluxLightModule/readPPMdata.cpp"
/************************************ 
 * Copyright (C) 2020 Marina Egner <info@sheepindustries.de>
 *
 * This program is free software: you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the 
 * Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see <https://www.gnu.org/licenses/>.
 ************************************/

#include "readPPMdata.h"							// Include own header file
#include "ppmToSwitches.h"							// Library to evaluate switch position from ppm value
#include "tools.h" 									// Get filter class from there

#define INTERRUPT_BUFFERSIZE 8						// Max Buffersize of ISR; Max is 255
#define PPM_START_MIN 850							// Minimun for start impuls
#define PPM_START_MAX 980							// Maximum for start impuls
#define PPM_HIGH_MIN 700							// Minimun for High impuls
#define PPM_HIGH_MAX 2200							// Maximun for High impuls
#define CHANNEL1 0									// Array number of Multiswitch
#define CHANNEL2 1									// Array number of Multiswitch
#define MAX_TIME_SIGNAL 5000						// 2000ms Maximum time for signal to change
#define MAX_LAYER 4
#define MIN_LAYER 1

// Variables
uint8_t channelLayer = 1;

struct Channel {
	volatile uint16_t servoValue = 0;				// Saves time difference of this channel
	volatile uint32_t lastMicros = 0;				// Time since last interrupt
	volatile uint32_t lastValidMillis = 0;			// Time since last valid save in interrupt
};

Channel interrupt[2];

// Classes
EdgeEvaluation edgeEval[2];									// Filter Function for Potis


/***************************************************
 * PPM Signal have a range from 1000ms to 2000ms
 * So 3 stages should be 1000/1500/2000 => UP/MID/DOWN
 * invertDirection is optional to turn the direction of UP and DOWN
 ***************************************************/

void initInterrupts(uint8_t pinPPMChannel1, uint8_t pinPPMChannel2) {
	attachInterrupt(digitalPinToInterrupt(pinPPMChannel1), ppmServo1Interrupt, CHANGE);		//Setup Interrupt
	attachInterrupt(digitalPinToInterrupt(pinPPMChannel2), ppmServo2Interrupt, CHANGE);		//Setup Interrupt
}

void ppmServo1Interrupt(){
	volatile uint32_t nMicros = micros(); 							// Store current time
	volatile uint32_t nDifference = (nMicros - interrupt[0].lastMicros); //Calc time since last Change
	if((nDifference > PPM_HIGH_MIN) && (nDifference < PPM_HIGH_MAX)) { // Filter HIGH Impulse | HIGH if time is between 700 and 2200 
		interrupt[0].servoValue = nDifference;								// Store current time difference to value
		interrupt[0].lastValidMillis = millis();						//save time of the last valid signal
	}
	interrupt[0].lastMicros = nMicros;								//save time for next interrupt
	return;
}

void ppmServo2Interrupt(){
	volatile uint32_t nMicros = micros(); 							// Store current time
	volatile uint32_t nDifference = (nMicros - interrupt[1].lastMicros); //Calc time since last Change
	if((nDifference > PPM_HIGH_MIN) && (nDifference < PPM_HIGH_MAX)) { // Filter HIGH Impulse | HIGH if time is between 700 and 2200 
		interrupt[1].servoValue = nDifference;								// Store current time difference to value
		interrupt[1].lastValidMillis = millis();						//save time of the last valid signal
	}
	interrupt[1].lastMicros = nMicros;								//save time for next interrupt
	return;
}

bool checkChannelStatus(uint8_t multiSwitch) {
	if((millis()-interrupt[multiSwitch].lastValidMillis) >= (uint16_t)MAX_TIME_SIGNAL) return false;
	return true;
}

uint8_t getChannel1Switch(uint8_t fallbackValue) {
	if(!checkChannelStatus(CHANNEL1)) return fallbackValue;				// return fallback if channel does not respond
	return ppmToSwitchStages(interrupt[CHANNEL1].servoValue);
}

uint8_t getChannel2Switch(uint8_t fallbackValue) {
	if(!checkChannelStatus(CHANNEL2)) return fallbackValue;				// return fallback if channel does not respond
	return ppmToSwitchStages(interrupt[CHANNEL2].servoValue);
}

uint8_t getCurrentLayer(uint8_t channelPosition) {
	bool switchUpPosition = false;
	bool switchDownPosition = false;
	if(channelPosition == DIRECTION_UP) bool switchUpPosition = true;
	if(channelPosition == DIRECTION_DOWN) bool switchDownPosition = true;

	bool switchUpEdge = edgeEval[0].readEdge(switchUpPosition);
	bool switchDownEdge = edgeEval[1].readEdge(switchDownPosition);

	if(switchUpEdge) channelLayer++;
	if(switchDownEdge) channelLayer--;
	if(channelLayer > MAX_LAYER) channelLayer = MAX_LAYER;
	if(channelLayer < MIN_LAYER) channelLayer = MIN_LAYER;
	return channelLayer;
}

EdgeEvaluation edgeEval2[8];
void mapSwitchToState(uint8_t layer, uint8_t channelValue, bool* stateSwitchUp, bool* stateSwitchDown) {
	bool switchUpPosition = false;
	bool switchDownPosition = false;
	if(channelValue == DIRECTION_UP) bool switchUpPosition = true;
	if(channelValue == DIRECTION_DOWN) bool switchDownPosition = true;

	uint8_t secondLayer = layer + MAX_LAYER;
	bool switchUpEdge = edgeEval2[layer].readEdge(switchUpPosition);
	bool switchDownEdge = edgeEval2[secondLayer].readEdge(switchDownPosition);

	if(switchUpEdge) *stateSwitchUp != *stateSwitchUp;
	if(switchDownEdge) *stateSwitchDown != *stateSwitchDown;
}

