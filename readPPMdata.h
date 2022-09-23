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

#ifndef _READ_PPM_DATA_H_
#define _READ_PPM_DATA_H_

#include "Arduino.h"

#define DIRECTION_UP 1
#define DIRECTION_MID 2
#define DIRECTION_DOWN 3

void initInterrupts(uint8_t pinPPMChannel1, uint8_t pinPPMChannel2);	// Function to attach all interrupt service routines (ISR)
void ppmServo1Interrupt();							// ISR for interrupt of servo PPM signal
void ppmServo2Interrupt();							// ISR for interrupt of servo PPM signal
bool checkChannelStatus(uint8_t multiSwitch);		// Function to check the Status of the Channel (If signal is lost)
uint8_t getChannel1Switch(uint8_t fallbackValue);	// Function to get the value of the Switches from Channel 1
uint8_t getChannel2Switch(uint8_t fallbackValue);	// Function to get the value of the Switches from Channel 2
uint8_t getCurrentLayer(uint8_t channelPosition);
void mapSwitchToState(uint8_t layer, uint8_t channelValue, bool* stateSwitchUp, bool* stateSwitchDown);

#endif