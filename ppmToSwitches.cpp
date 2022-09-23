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
#include "ppmToSwitches.h"

/***************************************************
 * PPM Signal have a range from 1000ms to 2000ms
 * So 3 stages should be 1000/1500/2000 => UP/MID/DOWN
 * invertDirection is optional to turn the direction of UP and DOWN
 ***************************************************/
uint8_t ppmToSwitchStages(uint16_t signal, bool invertDirection) {
	uint8_t dynDirectionUp;
	uint8_t dynDirectionDown;
	if(invertDirection) {						//if switch output is inverted
		dynDirectionUp = DIRECTION_DOWN;			//then assign opposite direction
		dynDirectionDown = DIRECTION_UP;
	} else {
		dynDirectionUp = DIRECTION_UP;			//else assign same direction
		dynDirectionDown = DIRECTION_DOWN;
	}
	if((signal >= 750) && (signal <= 1250)) {	//if signal is at 1000ms ±250ms 
		return dynDirectionUp;								//return 1 if signal is at lower end
	} else if(signal <= 1750) {					//else if signal is at 1500ms ±250ms
		return DIRECTION_MID;								//return 2 if signal is at middle
	} else if(signal <= 2250) {					//else if signal is at 2000ms ±250ms
		return dynDirectionDown;								//return 3 if signal is at upper end
	} else {									//else signal is <750 or >2250
		return 0;								//return 0 cause signal is out of bound | error
	}
}

/***************************************************
 * PPM Signal have a range from 1000ms to 2000ms
 * 3 Stages on 2 signals
 * If first signal have ~1000 the switch is in up position
 * If both signals have ~1700 the switch is in middle position
 * If second signal have ~1000 the switch is in down position
 ***************************************************/
uint8_t ppm2ToSwitch3Stages(uint16_t signal1, uint16_t signal2) {
	if((signal1 >= 750) && (signal1 <= 1250)) {			//if signal is at 1000ms ±250ms 
		return DIRECTION_UP;										//return 1 if signal is at lower end
	} else if((signal2 >= 750) && (signal2 <= 1250)) {	//else if signal is at 2000ms ±250ms
		return DIRECTION_DOWN;										//return 3 if signal is at upper end
	} else if((signal1 >= 1500) && (signal2 >= 1500) && (signal1 <= 2000) && (signal2 <= 2000)) {	//else if signal is at 1500ms ±250ms
		return DIRECTION_MID;										//return 2 if signal is at middle
	} else {											//else signal is <750 or >2250
		return 0;										//return 0 cause signal is out of bound | error
	}
}

/***************************************************
 * PPM Signal have a range from 1000ms to 2000ms
 * input Range of signal is defined by inMin (~1000) inMax (~2000)
 * returns a value from outMin to outMax
 * if calculation fails a zero returns
 ***************************************************/
int32_t ppmServoToRange(int32_t signal, int32_t inMin, int32_t inMax, int32_t outMin, int32_t outMax) {
	int32_t dynResult;
	if(inMin != inMax) {						//if Min and Max are equal abbort calculation cause of divide by zero
		dynResult = (signal - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
		if(dynResult > outMax) dynResult = outMax;
		if(dynResult < outMin) dynResult = outMin;
		return dynResult; 
	} else {
		return 0;
	}
}
