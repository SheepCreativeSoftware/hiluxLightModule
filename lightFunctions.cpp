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

#include "lightFunctions.h"
#include "tools.h"
#include <SoftPWM.h>							// https://github.com/bhagman/SoftPWM


Blink flasher[1];

void setFlasherLight(bool leftFlasherState, bool rightFlasherState, bool hazardState, bool* outLeftLight, bool* outRightLight, uint16_t flashFrequency) {
	if(hazardState) {
		bool blinkerState = flasher[1].blink(flashFrequency);
		*outLeftLight = blinkerState;
		*outRightLight = blinkerState;
	} else if(leftFlasherState) {
		*outLeftLight = flasher[1].blink(flashFrequency);
		*outRightLight = false;
	} else if(rightFlasherState) {
		*outLeftLight = false;
		*outRightLight = flasher[1].blink(flashFrequency);
	} else if((*outLeftLight || *outRightLight) && !flasher[1].blink(flashFrequency)) {
		*outLeftLight = false;
		*outRightLight = false;
	} else if (!*outLeftLight && !*outRightLight) {
		flasher[1].resetBlink();
	}
}

void initLightOutput() {
	SoftPWMBegin(); 					//Init Soft PWM Lib
}

void setupLightOutput(uint8_t pin, uint16_t fadeOnTime, uint16_t fadeOffTime) {
	SoftPWMSet(pin, SOFT_PWM_LOW);     	//Create and set pin to 0
	SoftPWMSetFadeTime(pin, fadeOnTime, fadeOffTime); //Set Fade on/off time for output
}

void setBooleanLight(uint8_t pin, bool state, uint8_t highValue) {
	if(state) SoftPWMSet(pin, highValue);
	if(!state) SoftPWMSet(pin, SOFT_PWM_LOW);
}
