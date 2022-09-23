#line 1 "/home/magraina/projects/hiluxLightModule/ioMapping.h"
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

#ifndef _IO_MAPPING_H_
#define _IO_MAPPING_H_

/************************************
 * Definition IO Pins
 ************************************/
// TODO: setup correct pins
//Pinout Arduino Micro:
//HW Serial 0+1 | I2C 2+3 | PWM 3, 5, 6, 9, 10, 11, 13 | LED 13
//Servo Lib deactivates PWM functionality on pins 9 and 10

//Inputs
#define inFunction1ControlPPM 2            //PPM Signal from Remote Control Extension | Interrupt Needed
#define inFunction2ControlPPM 3            //PPM Signal from Remote Control Extension | Interrupt Needed
#define inSoundPPM 7               			//Soundmodul Servo Signal from Receiver  | Interrupt Needed
// Pin 0+1+4 reserved for RS485!
//Pinbelegung
#define inChannel1PPM 0
#define inChannel2PPM 1


//Outputs
#define outStatusLed 13					//Arduino status LED output Pin
#define outGearShifter 3
#define outParkingLight 5
#define outLightFront 6
#define outFlasherLeft 7
#define outFlasherRight 8
#define outAuxLight 9     //Pin 9 und 10 ohne PWM wenn Servo attached
#define outPositionLightFront 11 
//Free IOs 18, 19

#endif