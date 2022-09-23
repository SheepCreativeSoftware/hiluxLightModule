#line 1 "/home/magraina/projects/hiluxLightModule/config.h"
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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define HEADLIGHT_IS_PARKING true			// Head light is also parking light (dimmed)
#define HEADLIGHT_IS_HIGHBEAM false			// High => Low and High Beam on both Head Light output Pins | False => Seperate Pins for High and Low Beam 
#define HEADLIGHT_PARKING_VALUE 5			// 0-255 Value for combined operation of Headlight
#define HEADLIGHT_LOWBEAM_VALUE 255			// 0-255 Value for combined operation of Headlight
#define HEADLIGHT_HIGHBEAM_VALUE 255		// 0-255 Value for combined operation of Headlight
#define HIGH_BEAM_FLASH_FREQUENCY 800		// Time frequency for head beam to flash
#define BLINKER_FREQUENCY 1000				// Time frequency for Flasher to flash
#define LIGHT_FADE_ON_TIME 200				// 200ms Fade on time for the Light
#define LIGHT_FADE_OFF_TIME 200				// 200ms Fade off time for the Light
#define PARKING_DIMM 100					// 0-255 Value for dimming the parking light
#define NORMAL_LIGHT_DIMMING 255			// 0-255 Value for all light when active
#define STARTER_DIMM_DIVISOR 5				// Divisor for Dimming function
#define STARTER_DIMM_MULTI1 2				// 0-255 MAX Value for all light when active starter is activ
#define SERIAL_COM true						// Activate Communication to other modules via Serial Port
#if (SERIAL_COM == true)
	#define SERIAL_TURNAROUND 20
#endif
//Change this value for different debuging levels
#define DEBUGLEVEL 6						//1 = Status LED | >2 = Serial Monitor

#endif