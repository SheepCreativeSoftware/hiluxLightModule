#include <Servo.h>

//Pinbelegung
#define kanal1 0
#define kanal2 1
#define schaltGetriebe 3
#define lichtStand 5
#define lichtFront 6
#define blinkerL 7
#define blinkerR 8
#define zusatzLicht 9     //Pin 9 und 10 ohne PWM wenn Servo attached
#define posLichtFront 11 

// Variablen die in Interrupt Verwendet werden mit "volatile"
volatile long lastChange1 = 0;
volatile long lastChange2 = 0;
volatile int impKanal1 = 0;
volatile int impKanal2 = 0;
volatile int lastImpKanal1 = 1450;
volatile int lastImpKanal2 = 1450;
volatile int valueKanal[5] = { 0 };
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
boolean output1 = false;
boolean laststate2 = false;
boolean output2 = false;
boolean laststate3 = false;
boolean output3 = false;
boolean laststate4 = false;
boolean output4 = false;
boolean laststate5 = false;
boolean laststate6 = false;
boolean laststate7 = false;
int gang = 2;

boolean standLicht = false;
boolean abblendLicht = false;
boolean fernLicht = false;


Servo schaltServo;

void setup() {
  // put your setup code here, to run once:
  pinMode(kanal1, INPUT);
  pinMode(kanal2, INPUT);
  pinMode(lichtStand, OUTPUT);
  pinMode(lichtFront, OUTPUT);
  pinMode(posLichtFront, OUTPUT);
  pinMode(zusatzLicht, OUTPUT);
  pinMode(blinkerL, OUTPUT);
  pinMode(blinkerR, OUTPUT);
  pinMode(schaltGetriebe, OUTPUT);
  schaltServo.attach(schaltGetriebe);
  schaltServo.write(90);
  //Serial.begin(57600);
  attachInterrupt(digitalPinToInterrupt(kanal1), kanal1Interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(kanal2), kanal1Interrupt, CHANGE);
  
}

void kanal1Interrupt() {
  volatile long nMicros = micros(); //Speichere aktuelle Zeit
  volatile long nDifference = (nMicros - lastChange1); //Errechne Zeit seit der letzten Flanke

  if((nDifference > 700) && (nDifference < 2200)) { //HIGH Impuls herausfiltern | Wenn Zeit zwischen 700 und 2200 handelt es sich um einen HIGH Impuls
    impKanal1 = nDifference;
    if(impKanal1 > lastImpKanal1+50) {
      impKanal1 = lastImpKanal1+50;
    } else if(impKanal1 < lastImpKanal1-50) {
      impKanal1 = lastImpKanal1-50;
    }
    lastImpKanal1 = impKanal1;
  } else if ((nDifference < 700) || (nDifference > 20000)) {
    errorLevel1 = true;
  }
  lastChange1 = nMicros;// Speichere aktuelle Zeit für den nächsten Interrupt
  volatile boolean state;
  if(impKanal1 >= 1650) {
    state = 1;
  } else if(impKanal1 <= 1250) {
    state = 2;
  } else {
    state = 0;
  }
  if((state == 2) && (laststate == 0)) {
    ebene++;
    if(ebene >=5) ebene = 1;
    laststate = state;
  } else if((state == 1) && (laststate == 0)) {
    ebene--;
    if(ebene <=0) ebene = 4;
    laststate = state;
  } else {
    laststate = state;
  }
}

void kanal2Interrupt() {
  volatile long nMicros = micros(); //Speichere aktuelle Zeit
  volatile long nDifference = (nMicros - lastChange2); //Errechne Zeit seit der letzten Flanke
  if((nDifference > 700) && (nDifference < 2200)) { //HIGH Impuls herausfiltern | Wenn Zeit zwischen 700 und 2200 handelt es sich um einen HIGH Impuls
    impKanal2 = nDifference;
    if(impKanal2 > lastImpKanal2+50) {
      impKanal2 = lastImpKanal2+50;
    } else if(impKanal2 < lastImpKanal2-50) {
      impKanal2 = lastImpKanal2-50;
    }
    lastImpKanal2 = impKanal2;
  } else if ((nDifference < 700) || (nDifference > 20000)) {
    errorLevel2 = true;
  }
  lastChange2 = nMicros;// Speichere aktuelle Zeit für den nächsten Interrupt
   
  if(impKanal2 >= 1650) {
    valueKanal[ebene] = 2;
  } else if(impKanal2 <= 1250) {
    valueKanal[ebene] = 1;
  } else {
    valueKanal[ebene] = 0;
  }
}


void loop() {
  if(errorLevel1 || errorLevel2) {
    if(millis() >= firstErrorTime+600000) {
      errorLevel = true;
    }
    if(!firstError) {
      firstError = true;
      firstErrorTime = millis();
    }
    
  } else {
    firstError = false;
    errorLevel = false;
  }
  if(errorLevel) {
    
  } else {
    // put your main code here, to run repeatedly:
   /* int impKanal1 = pulseIn(kanal1, HIGH, 20000);
    int impKanal2 = pulseIn(kanal2, HIGH, 20000);
    if((impKanal1 <= 800) || (impKanal1 >= 2500)) impKanal1 = 1450;
    if((impKanal2 <= 800) || (impKanal2 >= 2500)) impKanal2 = 1450;
    if(impKanal1 > lastImpKanal1+50) {
      impKanal1 = lastImpKanal1+50;
    } else if(impKanal1 < lastImpKanal1-50) {
      impKanal1 = lastImpKanal1-50;
    }
    
    lastImpKanal1 = impKanal1;
    if(impKanal2 > lastImpKanal2+50) {
      impKanal2 = lastImpKanal2+50;
    } else if(impKanal2 < lastImpKanal2-50) {
      impKanal2 = lastImpKanal2-50;
    }
     lastImpKanal2 = impKanal2;
  
    if(impKanal1 >= 1650) {
      state = 1;
    } else if(impKanal1 <= 1250) {
      state = 2;
    } else {
      state = 0;
    }
    if((state == 2) && (laststate == 0)) {
      ebene++;
      if(ebene >=5) ebene = 1;
      laststate = state;
    } else if((state == 1) && (laststate == 0)) {
      ebene--;
      if(ebene <=0) ebene = 4;
      laststate = state;
    } else {
      laststate = state;
    }
    
    if(impKanal2 >= 1650) {
      valueKanal[ebene] = 2;
    } else if(impKanal2 <= 1250) {
      valueKanal[ebene] = 1;
    } else {
      valueKanal[ebene] = 0;
    }*/
    /*Serial.println(impKanal1);
    Serial.println(impKanal2);
    Serial.println("ebene");
    Serial.println(ebene);
    Serial.println(valueKanal[ebene]);*/
    // Lichtfunktionen Stand- und Abblendlicht
    boolean state0;
    if(valueKanal[4] == 1) {
      state0 = 1;
    } else{
      state0 = 0;
    }
    if((state0 == 1) && (laststate0 == 0)){
      if((standLicht == true) && (abblendLicht == true)) {
        standLicht = !standLicht;
        abblendLicht = !abblendLicht;
        digitalWrite(lichtStand, LOW);
        digitalWrite(posLichtFront, LOW);
      } else if((standLicht == true) && (abblendLicht == false)) {
        abblendLicht = !abblendLicht;
      } else if((standLicht == false) && (abblendLicht == false)) {
        standLicht = !standLicht;
        analogWrite(lichtStand,100);
        analogWrite(posLichtFront, 100);
      }
      laststate0 = state0;
    }else{
      laststate0 = state0;
    }
  
      // Funktion Fernlicht
      boolean state7;
    if(valueKanal[4] == 2) {
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
          digitalWrite(lichtFront, LOW);
        } else if(standLicht == true) {
          digitalWrite(lichtFront, LOW);
        }
      } else if(abblendLicht == true) {
        analogWrite(lichtFront, 100);
      }
    } else if(fernLicht == true) {
    digitalWrite(lichtFront, HIGH);
    } 
  
      //Blinker Warn flankenauswertung  output1
    boolean state1;
    if(valueKanal[3] == 2) {
      state1 = 1;
    } else{
      state1 = 0;
    }
    if((state1 == 1) && (laststate1 == 0)) {
      output1 = !output1;
      laststate1 = state1;
    } else {
      laststate1 = state1;
    }
      //Blinker Links flankenauswertung  output2
    boolean state2;
    if(valueKanal[2] == 1) {
      state2 = 1;
    } else{
      state2 = 0;
    }
    if((state2 == 1) && (laststate2 == 0)) {
      output2 = !output2;
      output3 = 0;
      laststate2 = state2;
    } else {
      laststate2 = state2;
    }
    //Blinker Rechts flankenauswertung  output3
    boolean state3;
    if(valueKanal[2] == 2) {
      state3 = 1;
    } else{
      state3 = 0;
    }
    if((state3 == 1) && (laststate3 == 0)) {
      output3 = !output3;
      output2 = 0;
      laststate3 = state3;
    } else {
      laststate3 = state3;
    }
    
      // Ausfürung der Blinkfunktion Warn, Links und Rechts
    if(output1){
      digitalWrite(blinkerL,blink());
      digitalWrite(blinkerR,blink());
    } else {
      if(output2){
        digitalWrite(blinkerL,blink());
        digitalWrite(blinkerR, LOW);
      } else if(output3){
        digitalWrite(blinkerR,blink());
        digitalWrite(blinkerL,LOW);
      } else {
        digitalWrite(blinkerR, LOW);
        digitalWrite(blinkerL,LOW);
        blinkOnTime = 0;
      }
    }
  
     //Zusatzlicht 1 (flankenauswertung)
    boolean state4 = 0;
    if(valueKanal[3] == 1) {
      state4 = 1;
    } else{
      state4 = 0;
    }
    if((state4 == 1) && (laststate4 == 0)) {
      output4 = !output4;
      digitalWrite(zusatzLicht, output4);
      laststate4 = state4;
    } else {
      laststate4 = state4;
    }
  
    //Gänge Hoch und herunterschalten
    boolean state5;
    if(valueKanal[1] == 2) {
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
    if(valueKanal[1] == 1) {
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
      schaltServo.attach(schaltGetriebe);
    } else {
      schaltServo.detach();
      digitalWrite(schaltGetriebe, LOW);
    }*/
    switch (gang) {
      case 1:
        schaltServo.write(0);
        
        break;
      case 2:
        schaltServo.write(90);
  
        break;
      case 3:
        schaltServo.write(180);
        break;
    }
    /*interval++;
    if((millis() -1000) >= lastMillis) {
      lastMillis = millis();
      lastInterval = interval;
      interval = 0;
    }
    //Serial.println("Interval");
    //Serial.println(lastInterval);*/
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
