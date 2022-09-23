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
 
#ifndef _PPM_TO_SWITCHES_h_
#define _PPM_TO_SWITCHES_h_
//Definition
#include "Arduino.h"

#define DIRECTION_UP 1
#define DIRECTION_MID 2
#define DIRECTION_DOWN 3
#define PPM_INVERT 1

//Functions
uint8_t ppmToSwitchStages(uint16_t signal, bool invertDirection = 0);	// Function to evaluate the ppm signal of a switch
uint8_t ppm2ToSwitch3Stages(uint16_t signal1, uint16_t signal2);		// Function to evaluate the ppm signal of a 3 stages switch with two signals
int32_t ppmServoToRange(int32_t signal, int32_t inMin = 1060, int32_t inMax = 1980, int32_t outMin = 0, int32_t outMax = 1023); // Function to evaluate the ppm signal from a servo

#endif