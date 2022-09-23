# 1 "/home/magraina/projects/hiluxLightModule/hiluxLightModule.ino"
/************************************ 
 * hiluxLightModule v0.2.0
 * Date: 24.09.2022 | 00:06
 * <Hilux Light Module>
 * Copyright (C) 2020 Marina Egner <info@sheepCreativeStudios.de>
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

/************************************
 * Configuration Programm
 ************************************/
# 23 "/home/magraina/projects/hiluxLightModule/hiluxLightModule.ino" 2
# 24 "/home/magraina/projects/hiluxLightModule/hiluxLightModule.ino" 2
/************************************
 * Include Module and Library Files
 ************************************/

# 29 "/home/magraina/projects/hiluxLightModule/hiluxLightModule.ino" 2
# 30 "/home/magraina/projects/hiluxLightModule/hiluxLightModule.ino" 2
# 31 "/home/magraina/projects/hiluxLightModule/hiluxLightModule.ino" 2

//Setup Serial and check if Board is UNO with one Serial or Leonardo/Micro with to Serials
# 43 "/home/magraina/projects/hiluxLightModule/hiluxLightModule.ino"
// Variablen die in Interrupt Verwendet werden mit "volatile"
volatile long lastChange1 = 0;
volatile long lastChange2 = 0;
volatile int impKanal1 = 0;
volatile int impKanal2 = 0;
volatile int lastImpKanal1 = 1450;
volatile int lastImpKanal2 = 1450;
volatile boolean errorLevel1 = false;
volatile boolean errorLevel2 = false;
boolean laststate = false;
volatile int ebene = 1;

boolean errorLevel = false;
boolean firstError = false;
unsigned long firstErrorTime = 0;

unsigned long blinkOnTime = 0;
boolean laststate0 = false;
boolean laststate1 = false;
boolean hazardState = false;
boolean laststate2 = false;
boolean leftFlasherState = false;
boolean laststate3 = false;
boolean rightFlasherState = false;
boolean laststate4 = false;
boolean output4 = false;
boolean laststate5 = false;
boolean laststate6 = false;
boolean laststate7 = false;
int gang = 2;

boolean standLicht = false;
boolean abblendLicht = false;
boolean fernLicht = false;


Servo shifterServo;

void setup() {
 // put your setup code here, to run once:
 /************************************
	* Setup Inputs 
	************************************/
 pinMode(0, 0x2);
 pinMode(1, 0x2);
 /************************************
	* Setup Outputs 
	************************************/
 initLightOutput();
 setupLightOutput(5, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(6, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(11, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(9 /*Pin 9 und 10 ohne PWM wenn Servo attached*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(7, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(8, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 setupLightOutput(9 /*Pin 9 und 10 ohne PWM wenn Servo attached*/, 200 /* 200ms Fade on time for the Light*/, 200 /* 200ms Fade off time for the Light*/);
 pinMode(3, 0x1);
 shifterServo.attach(3);
 shifterServo.write(90);
 /************************************
	* Setup Functions
	************************************/
 initInterrupts(0, 1);

 //debuggingInit(DEBUGLEVEL, outStatusLed);


}




uint8_t channelValue[5] { 0 };

void loop() { // put your main code here, to run repeatedly:
 uint8_t channel1Position = getChannel1Switch(2);

 uint8_t currentlayer = getCurrentLayer(channel1Position);

 channelValue[currentlayer] = getChannel2Switch(2);

 // mapSwitchToState(LAYER1, channelValue[LAYER1], &stateSwitchUp, &stateSwitchDown);
 // mapSwitchToState(LAYER2, channelValue[LAYER2], &stateSwitchUp, &stateSwitchDown);
 // mapSwitchToState(LAYER3, channelValue[LAYER3], &stateSwitchUp, &stateSwitchDown);
 // mapSwitchToState(LAYER4, channelValue[LAYER4], &stateSwitchUp, &stateSwitchDown);
 // Lichtfunktionen Stand- und Abblendlicht
 boolean state0;
 if(channelValue[4] == 3) {
   state0 = 1;
 } else{
   state0 = 0;
 }
 if((state0 == 1) && (laststate0 == 0)){
   if((standLicht == true) && (abblendLicht == true)) {
  standLicht = !standLicht;
  abblendLicht = !abblendLicht;
  setBooleanLight(5, 0x0, 255 /* 0-255 Value for all light when active*/);
  setBooleanLight(11, 0x0, 255 /* 0-255 Value for all light when active*/);
   } else if((standLicht == true) && (abblendLicht == false)) {
  abblendLicht = !abblendLicht;
   } else if((standLicht == false) && (abblendLicht == false)) {
  standLicht = !standLicht;
  setBooleanLight(5, 0x1, 100 /* 0-255 Value for dimming the parking light*/);
  setBooleanLight(11, 0x1, 100 /* 0-255 Value for dimming the parking light*/);
   }
   laststate0 = state0;
 }else{
   laststate0 = state0;
 }

   // Funktion Fernlicht
   boolean state7;
 if(channelValue[4] == 1) {
   state7 = 1;
 } else{
   state7 = 0;
 }
 if((state7 == 1) && (laststate7 == 0)) {
   fernLicht = !fernLicht;
   laststate7 = state7;
 } else {
   laststate7 = state7;
 }
 // Licht an der Front satndlicht ablendlicht oder fernlicht
 if(fernLicht == false) {
   if(abblendLicht == false) {
  if(standLicht == false) {
    setBooleanLight(6, 0x0, 255 /* 0-255 Value for all light when active*/);
  } else if(standLicht == true) {
    setBooleanLight(6, 0x0, 255 /* 0-255 Value for all light when active*/);
  }
   } else if(abblendLicht == true) {
  setBooleanLight(6, 0x1, 100 /* 0-255 Value for dimming the parking light*/);
   }
 } else if(fernLicht == true) {
 setBooleanLight(6, 0x1, 255 /* 0-255 Value for all light when active*/);
 }

   //Blinker Warn flankenauswertung  hazardState
 boolean state1;
 if(channelValue[3] == 3) {
   state1 = 1;
 } else{
   state1 = 0;
 }
 if((state1 == 1) && (laststate1 == 0)) {
   hazardState = !hazardState;
   laststate1 = state1;
 } else {
   laststate1 = state1;
 }
   //Blinker Links flankenauswertung  leftFlasherState
 boolean state2;
 if(channelValue[2] == 3) {
   state2 = 1;
 } else{
   state2 = 0;
 }
 if((state2 == 1) && (laststate2 == 0)) {
   leftFlasherState = !leftFlasherState;
   rightFlasherState = 0;
   laststate2 = state2;
 } else {
   laststate2 = state2;
 }
 //Blinker Rechts flankenauswertung  rightFlasherState
 boolean state3;
 if(channelValue[2] == 1) {
   state3 = 1;
 } else{
   state3 = 0;
 }
 if((state3 == 1) && (laststate3 == 0)) {
   rightFlasherState = !rightFlasherState;
   leftFlasherState = 0;
   laststate3 = state3;
 } else {
   laststate3 = state3;
 }

 // Ausfürung der Blinkfunktion Warn, Links und Rechts
 bool leftFlasherOut = false;
 bool rightFlasherOut = false;
 setFlasherLight(leftFlasherState, rightFlasherState, hazardState, &leftFlasherOut, &rightFlasherOut, 1000 /* Time frequency for Flasher to flash*/);
 setBooleanLight(7, leftFlasherOut, 255 /* 0-255 Value for all light when active*/);
 setBooleanLight(8, rightFlasherOut, 255 /* 0-255 Value for all light when active*/);

  //Zusatzlicht 1 (flankenauswertung)
 boolean state4 = 0;
 if(channelValue[3] == 1) {
   state4 = 1;
 } else{
   state4 = 0;
 }
 if((state4 == 1) && (laststate4 == 0)) {
   output4 = !output4;
   setBooleanLight(9 /*Pin 9 und 10 ohne PWM wenn Servo attached*/, output4, 255 /* 0-255 Value for all light when active*/);
   laststate4 = state4;
 } else {
   laststate4 = state4;
 }

 //Gänge Hoch und herunterschalten
 boolean state5;
 if(channelValue[1] == 1) {
   state5 = 1;
 } else{
   state5 = 0;
 }
 if((state5 == 1) && (laststate5 == 0)) {
   gang++;
   if(gang >=4) gang = 3;
   laststate5 = state5;
 } else {
   laststate5 = state5;
 }
 boolean state6 = 0;
 if(channelValue[1] == 3) {
   state6 = 1;
 } else{
   state6 = 0;
 }
 if((state6 == 1) && (laststate6 == 0)) {
   gang--;
   if(gang <= 0) gang = 1;
   laststate6 = state6;
 } else {
   laststate6 = state6;
 }
 /*if(ebene == 1) {
	  shifterServo.attach(outGearShifter);
	} else {
	  shifterServo.detach();
	  digitalWrite(outGearShifter, LOW);
	}*/
 switch (gang) {
   case 1:
  shifterServo.write(0);

  break;
   case 2:
  shifterServo.write(90);

  break;
   case 3:
  shifterServo.write(180);
  break;
 }

}

int blink() {
  if(blinkOnTime == 0){
 blinkOnTime = millis();
  }
  unsigned long blinkTime = millis() - blinkOnTime;
  if(blinkTime%1000 >= 500){
 return 1;
  } else {
 return 0;
  }

}
