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

#include "tools.h"

bool EdgeEvaluation::readEdge(bool input){
	if((input) && (!lastEdge)){
		lastEdge = true;
		return true;
	} else if((!input) && (lastEdge)){
		lastEdge = false;
		return false;
	} 
	return false;
}

bool DebounceEvaluation::readDebounced(bool input){
  uint32_t currentMillis = millis();

	if(input != lastInputState) {
		lastDebounceTime = currentMillis;
	}

  if((currentMillis - lastDebounceTime) > debounceDelay) {
    if(input != lastState) lastState = input;
  }

  lastInputState = input;

	return lastState;
}

int16_t Filter::filterValue(int16_t input, int16_t filterFactor, uint16_t filterTime){
	if((millis()%filterTime >= filterTime/2) && (doneFilter == false)) {
		lastValue = (input - lastValue) / filterFactor + lastValue; 
		doneFilter = true;
	} else if((millis()%filterTime < filterTime/2) && (doneFilter == true)) {
		doneFilter = false;
	}
	
	return lastValue;
}

uint8_t Blink::blink(uint16_t blinkTimeMillis) {
	if((blinkOnTime == 0) || (blinkOnTime > millis())){ 	//Reset blinkOnTime on startup and on overflow.
		blinkOnTime = millis();
	}
		uint32_t blinkTime = millis() - blinkOnTime;
	if(blinkTime%blinkTimeMillis >= blinkTimeMillis/2){ 	//ON/OFF Interval at half of Time.
		return 0;
	} else {
		return 1;
	}

}

void Blink::resetBlink() {
	blinkOnTime = 0;
}
