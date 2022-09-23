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

#ifndef _LIGHT_FUNCTIONS_H_
#define _LIGHT_FUNCTIONS_H_

#include "Arduino.h"

#define SOFT_PWM_HIGH 255
#define SOFT_PWM_LOW 0

bool directlyToOutput(bool lightState);
bool highBeamFlash(bool lightState, bool lightFlashState, uint16_t flashFrequency);
void setFlasherLight(bool leftFlasherState, bool rightFlasherState, bool hazardState, bool* outLeftLight, bool* outRightLight, uint16_t flashFrequency);
void initLightOutput();
void setupLightOutput(uint8_t pin, uint16_t fadeOnTime, uint16_t fadeOffTime);
void setBooleanLight(uint8_t pin, bool state, uint8_t highValue = SOFT_PWM_HIGH);

void setCombinedHeadlightAll(uint8_t pin,
							uint8_t parkingState,
							uint8_t lowBeamState,
							uint8_t highBeamState,
							uint8_t parkingOutValue,
							uint8_t lowBeamOutValue,
							uint8_t highBeamOutValue);
void setCombinedHeadlightHighOnly(uint8_t pin,
							uint8_t lowBeamState,
							uint8_t highBeamState,
							uint8_t lowBeamOutValue,
							uint8_t highBeamOutValue);

void setCombinedHeadlightParkOnly(uint8_t pin,
							uint8_t parkingState,
							uint8_t lowBeamState,
							uint8_t parkingOutValue,
							uint8_t lowBeamOutValue);
uint8_t starterDimming(bool active, uint8_t defaultDimValue, uint8_t divisor, uint8_t multiplier1);

#endif