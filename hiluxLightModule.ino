/************************************ 
 * hiluxLightModule v2.0.0
 * Date: 05.01.2025 | 20:34
 * <Hilux Light Module>
 * Copyright (C) 2025 Marina Egner <hello@sheepcs.de>
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

/************************************
 * Configuration Programm
 ************************************/
#include "config.h"								// Configuration File
#include "ioMapping.h"							// IO Mapping File
/************************************
 * Include Module and Library Files
 ************************************/

#include "readPPMdata.h"				// read Data from Buffer
#include "lightFunctions.h"

//Setup Serial and check if Board is UNO with one Serial or Leonardo/Micro with to Serials
#ifdef HAVE_HWSERIAL1								//if serial ports 1 exist then the arduino has more than one serial port
	#ifndef SerialUSB								//if not allready defined
		#define SerialUSB SERIAL_PORT_MONITOR		//then define monitor port
	#endif
#else
	#if (DEBUGLEVEL >1)								//if serial ports are the same debuging is not possible (for example on UNO)
		#define DEBUGLEVEL 1						//do not change!!!
	#endif
#endif

boolean lastChannel1DownState = false;
boolean hazardState = false;
boolean leftFlasherState = false;
boolean rightFlasherState = false;
boolean auxLightState = false;
boolean previousHighBeamState = false;
boolean parkingLightState = false;
boolean lowBeamState = false;
boolean highBeamState = false;

void setup() {
	// put your setup code here, to run once:
	/************************************
	* Setup Inputs 
	************************************/
	pinMode(inChannel1PPM, INPUT_PULLUP);
	pinMode(inChannel2PPM, INPUT_PULLUP);
	/************************************
	* Setup Outputs 
	************************************/
	initLightOutput();
	setupLightOutput(outParkingLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outLightFront, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outPositionLightFront, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outAuxLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outFlasherLeft, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outFlasherRight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	setupLightOutput(outAuxLight, LIGHT_FADE_ON_TIME, LIGHT_FADE_OFF_TIME);
	/************************************
	* Setup Functions
	************************************/
	initInterrupts(inChannel1PPM, inChannel2PPM);
}

#define LAYER1 1
#define LAYER2 2
uint8_t channelValue[3] { 0 };
boolean channel1StateDown = false;

void loop() { // put your main code here, to run repeatedly:
	channelValue[LAYER1] = getChannel1Switch(DIRECTION_MID);
	channelValue[LAYER2] = getChannel2Switch(DIRECTION_MID);

	mapSwitchToState(LAYER1, channelValue[LAYER1], &highBeamState, &channel1StateDown);
	mapSwitchToState(LAYER2, channelValue[LAYER2], &auxLightState, &hazardState);

	// Separate channel evaluation because of different edge evaluation
	boolean channel1DownDirection;
	if(channelValue[LAYER1] == DIRECTION_DOWN) {
		channel1DownDirection = 1;
	} else{
		channel1DownDirection = 0;
	}
	if((channel1DownDirection == 1) && (lastChannel1DownState == 0)){
		if((parkingLightState == true) && (lowBeamState == true)) {
			parkingLightState = !parkingLightState;
			lowBeamState = !lowBeamState;
			setBooleanLight(outParkingLight, LOW, NORMAL_LIGHT_DIMMING);
			setBooleanLight(outPositionLightFront, LOW, NORMAL_LIGHT_DIMMING);
		} else if((parkingLightState == true) && (lowBeamState == false)) {
			lowBeamState = !lowBeamState;
		} else if((parkingLightState == false) && (lowBeamState == false)) {
			parkingLightState = !parkingLightState;
			setBooleanLight(outParkingLight, HIGH, PARKING_DIMM);
			setBooleanLight(outPositionLightFront, HIGH, PARKING_DIMM);
		}
		lastChannel1DownState = channel1DownDirection;
	} else {
		lastChannel1DownState = channel1DownDirection;
	}

	// Set Light on front depending on different states
	if(highBeamState == false) {
		if(lowBeamState == true) {
			setBooleanLight(outLightFront, HIGH, HEADLIGHT_LOWBEAM_VALUE);
		} else {
			setBooleanLight(outLightFront, LOW, HEADLIGHT_LOWBEAM_VALUE);
		}
	} else if(highBeamState == true) {
		setBooleanLight(outLightFront, HIGH, HEADLIGHT_HIGHBEAM_VALUE);
	} 

	bool leftFlasherOut = false;
	bool rightFlasherOut = false;
	setFlasherLight(leftFlasherState, rightFlasherState, hazardState, &leftFlasherOut, &rightFlasherOut, BLINKER_FREQUENCY);
	setBooleanLight(outFlasherLeft, leftFlasherOut, NORMAL_LIGHT_DIMMING);
	setBooleanLight(outFlasherRight, rightFlasherOut, NORMAL_LIGHT_DIMMING);

	 // Aux Light
	setBooleanLight(outAuxLight, auxLightState, NORMAL_LIGHT_DIMMING);
}
