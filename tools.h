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

#ifndef _TOOLS_H_
#define _TOOLS_H_
//Definition
#include "Arduino.h"
#include <SoftPWM.h>							//https://github.com/bhagman/SoftPWM


//Classes
class EdgeEvaluation {
		bool lastEdge;
    public:
		bool readEdge(bool input);
};

class Filter {
		int16_t lastValue;
		bool doneFilter;
    public:
		int16_t filterValue(int16_t input, int16_t filterFactor = 20, uint16_t filterTime = 100);
};

class Blink {
	private:
		uint32_t blinkOnTime = 0;
	public:
		uint8_t blink(uint16_t blinkTimeMillis);
		void resetBlink(); // Reset Blink after usage for next usage
};

/*
class outputDefine {
		unsigned int outPinModus;
    public:
		
		void outputMode(int outPin, unsigned char modus);
		void outputMode(int outPin, unsigned char modus, int fadeUpTime, int fadeDownTime);		
};
//Modus pwmHW softPWM softFade digital
*/

#endif