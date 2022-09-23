#include <Arduino.h>
#line 1 "/home/magraina/projects/hiluxLightModule/hiluxLightModule.ino"
/************************************ 
 * hiluxLightModule v0.2.0
 * Date: 24.09.2022 | 00:06
 * <Hilux Light Module>
 * Copyright (C) 2020 Marina Egner <info@sheepCreativeStudios.de>
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
#include <Servo.h>

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

// Variablen die in Interrupt Verwendet werden mit "volatile"
volatile long lastChange1 = 0;
volatile long lastChange2 = 0;
volatile int impKanal1 = 0;
volatile int impKanal2 = 0;
volatile int lastImpKanal1 = 1450;
volatile int lastImpKanal2 = 1450;
volatile boolean errorLevel1 = false;
volatile boolean errorLevel2 = false;
boolean laststate = false;
volatile int ebene = 1;

boolean errorLevel = false;
boolean firstError = false;
unsigned long firstErrorTime = 0;

unsigned long blinkOnTime = 0;
boolean laststate0 = false;
boolean laststate1 = false;
boolean hazardState = false;
boolean laststate2 = false;
boolean leftFlasherState = false;
boolean laststate3 = false;
boolean rightFlasherState = false;
boolean laststate4 = false;
boolean output4 = false;
boolean laststate5 = false;
boolean laststate6 = false;
boolean laststate7 = false;
int gang = 2;

boolean standLicht = false;
boolean abblendLicht = false;
boolean fernLicht = false;


Servo shifterServo;

#line 81 "/home/magraina/projects/hiluxLightModule/hiluxLightModule.ino"
void setup();
#line 117 "/home/magraina/projects/hiluxLightModule/hiluxLightModule.ino"
void loop();
#line 294 "/home/magraina/projects/hiluxLightModule/hiluxLightModule.ino"
int blink();
#line 81 "/home/magraina/projects/hiluxLightModule/hiluxLightModule.ino"
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
	pinMode(outGearShifter, OUTPUT);
	shifterServo.attach(outGearShifter);
	shifterServo.write(90);
	/************************************
	* Setup Functions
	************************************/
	initInterrupts(inChannel1PPM, inChannel2PPM);
	#if (DEBUGLEVEL >=1)
	//debuggingInit(DEBUGLEVEL, outStatusLed);
	#endif

}
#define LAYER1 1
#define LAYER2 2
#define LAYER3 3
#define LAYER4 4
uint8_t channelValue[5] { 0 };

void loop() { // put your main code here, to run repeatedly:
	uint8_t channel1Position = getChannel1Switch(DIRECTION_MID);

	uint8_t currentlayer = getCurrentLayer(channel1Position);
	
	channelValue[currentlayer] = getChannel2Switch(DIRECTION_MID);

	// mapSwitchToState(LAYER1, channelValue[LAYER1], &stateSwitchUp, &stateSwitchDown);
	// mapSwitchToState(LAYER2, channelValue[LAYER2], &stateSwitchUp, &stateSwitchDown);
	// mapSwitchToState(LAYER3, channelValue[LAYER3], &stateSwitchUp, &stateSwitchDown);
	// mapSwitchToState(LAYER4, channelValue[LAYER4], &stateSwitchUp, &stateSwitchDown);
	// Lichtfunktionen Stand- und Abblendlicht
	boolean state0;
	if(channelValue[4] == DIRECTION_DOWN) {
	  state0 = 1;
	} else{
	  state0 = 0;
	}
	if((state0 == 1) && (laststate0 == 0)){
	  if((standLicht == true) && (abblendLicht == true)) {
		standLicht = !standLicht;
		abblendLicht = !abblendLicht;
		setBooleanLight(outParkingLight, LOW, NORMAL_LIGHT_DIMMING);
		setBooleanLight(outPositionLightFront, LOW, NORMAL_LIGHT_DIMMING);
	  } else if((standLicht == true) && (abblendLicht == false)) {
		abblendLicht = !abblendLicht;
	  } else if((standLicht == false) && (abblendLicht == false)) {
		standLicht = !standLicht;
		setBooleanLight(outParkingLight, HIGH, PARKING_DIMM);
		setBooleanLight(outPositionLightFront, HIGH, PARKING_DIMM);
	  }
	  laststate0 = state0;
	}else{
	  laststate0 = state0;
	}
  
	  // Funktion Fernlicht
	  boolean state7;
	if(channelValue[4] == DIRECTION_UP) {
	  state7 = 1;
	} else{
	  state7 = 0;
	}
	if((state7 == 1) && (laststate7 == 0)) {
	  fernLicht = !fernLicht;
	  laststate7 = state7;
	} else {
	  laststate7 = state7;
	}
	// Licht an der Front satndlicht ablendlicht oder fernlicht
	if(fernLicht == false) {
	  if(abblendLicht == false) {
		if(standLicht == false) {
		  setBooleanLight(outLightFront, LOW, NORMAL_LIGHT_DIMMING);
		} else if(standLicht == true) {
		  setBooleanLight(outLightFront, LOW, NORMAL_LIGHT_DIMMING);
		}
	  } else if(abblendLicht == true) {
		setBooleanLight(outLightFront, HIGH, PARKING_DIMM);
	  }
	} else if(fernLicht == true) {
	setBooleanLight(outLightFront, HIGH, NORMAL_LIGHT_DIMMING);
	} 
  
	  //Blinker Warn flankenauswertung  hazardState
	boolean state1;
	if(channelValue[3] == DIRECTION_DOWN) {
	  state1 = 1;
	} else{
	  state1 = 0;
	}
	if((state1 == 1) && (laststate1 == 0)) {
	  hazardState = !hazardState;
	  laststate1 = state1;
	} else {
	  laststate1 = state1;
	}
	  //Blinker Links flankenauswertung  leftFlasherState
	boolean state2;
	if(channelValue[2] == DIRECTION_DOWN) {
	  state2 = 1;
	} else{
	  state2 = 0;
	}
	if((state2 == 1) && (laststate2 == 0)) {
	  leftFlasherState = !leftFlasherState;
	  rightFlasherState = 0;
	  laststate2 = state2;
	} else {
	  laststate2 = state2;
	}
	//Blinker Rechts flankenauswertung  rightFlasherState
	boolean state3;
	if(channelValue[2] == DIRECTION_UP) {
	  state3 = 1;
	} else{
	  state3 = 0;
	}
	if((state3 == 1) && (laststate3 == 0)) {
	  rightFlasherState = !rightFlasherState;
	  leftFlasherState = 0;
	  laststate3 = state3;
	} else {
	  laststate3 = state3;
	}

	// Ausfürung der Blinkfunktion Warn, Links und Rechts
	bool leftFlasherOut = false;
	bool rightFlasherOut = false;
	setFlasherLight(leftFlasherState, rightFlasherState, hazardState, &leftFlasherOut, &rightFlasherOut, BLINKER_FREQUENCY);
	setBooleanLight(outFlasherLeft, leftFlasherOut, NORMAL_LIGHT_DIMMING);
	setBooleanLight(outFlasherRight, rightFlasherOut, NORMAL_LIGHT_DIMMING);

	 //Zusatzlicht 1 (flankenauswertung)
	boolean state4 = 0;
	if(channelValue[3] == DIRECTION_UP) {
	  state4 = 1;
	} else{
	  state4 = 0;
	}
	if((state4 == 1) && (laststate4 == 0)) {
	  output4 = !output4;
	  setBooleanLight(outAuxLight, output4, NORMAL_LIGHT_DIMMING);
	  laststate4 = state4;
	} else {
	  laststate4 = state4;
	}
  
	//Gänge Hoch und herunterschalten
	boolean state5;
	if(channelValue[1] == DIRECTION_UP) {
	  state5 = 1;
	} else{
	  state5 = 0;
	}
	if((state5 == 1) && (laststate5 == 0)) {
	  gang++;
	  if(gang >=4) gang = 3;
	  laststate5 = state5;
	} else {
	  laststate5 = state5;
	}
	boolean state6 = 0;
	if(channelValue[1] == DIRECTION_DOWN) {
	  state6 = 1;
	} else{
	  state6 = 0;
	}
	if((state6 == 1) && (laststate6 == 0)) {
	  gang--;
	  if(gang <= 0) gang = 1;
	  laststate6 = state6;
	} else {
	  laststate6 = state6;
	}
	/*if(ebene == 1) {
	  shifterServo.attach(outGearShifter);
	} else {
	  shifterServo.detach();
	  digitalWrite(outGearShifter, LOW);
	}*/
	switch (gang) {
	  case 1:
		shifterServo.write(0);
		
		break;
	  case 2:
		shifterServo.write(90);
  
		break;
	  case 3:
		shifterServo.write(180);
		break;
	}

}

int blink() {
  if(blinkOnTime == 0){
	blinkOnTime = millis();
  }
  unsigned long blinkTime = millis() - blinkOnTime;
  if(blinkTime%1000 >= 500){
	return 1;
  } else {
	return 0;
  }

}

