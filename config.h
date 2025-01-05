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

#define HEADLIGHT_LOWBEAM_VALUE 100			// 0-255 Value for combined operation of Headlight
#define HEADLIGHT_HIGHBEAM_VALUE 255		// 0-255 Value for combined operation of Headlight
#define BLINKER_FREQUENCY 1000				// Time frequency for Flasher to flash
#define LIGHT_FADE_ON_TIME 200				// 200ms Fade on time for the Light
#define LIGHT_FADE_OFF_TIME 200				// 200ms Fade off time for the Light
#define PARKING_DIMM 100					// 0-255 Value for dimming the parking light
#define NORMAL_LIGHT_DIMMING 255			// 0-255 Value for all light when active

#endif