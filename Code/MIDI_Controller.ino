/*************************************************************
  MIDI CONTROLLER

  by Felipe Gomes - 2018

  Version 1.0
 *************************************************************/
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE ();

// ========================  Led  ==================================//
int led = 8;

// ========================  Channel  ==================================//
byte channel = 1;
// ======================== Buttons ==================================//
const int nButtons = 1;               // Number of buttons
const int pinButtons[nButtons] = {2}; // Buttons pins
int flag[nButtons] = {};              // Flag to see if the button was pressed or not
int lastflag[nButtons] = {};      // 
//=====================================================================

// =================== Potentiometers / LDRs ================================//
const int nPots = 1;                  // Number of potentiometers
const int potentiometer[nPots] = {0}; // Potentiometers pins: A0 = 0; A1 = 1

int value[nPots] = {};                // Curent value 
int lastvalue[nPots] = {};            // Last value
//=====================================================================

void setup() {
  //MIDI.begin(1);
  // 115200 hairless MIDI
  Serial.begin(9600);
  for(int i = 0; i < nButtons; i++){
   pinMode(pinButtons[i], INPUT_PULLUP); 
  }
  pinMode(led, OUTPUT);
}

void loop() { 

  digitalWrite(led, LOW);
  
  momentary_Buttons();
  potentiometers();
  //ldr();
  
  //delay(100);
}

// Use this function if you are using DPDT
void dpdt_Buttons(){
  for(int i = 0; i < nButtons; i++){
    flag[i] = digitalRead(pinButtons[i]);
    if(flag[i] != lastflag[i]){
      lastflag[i] = flag[i];
      MIDI.sendControlChange(90+i,127,1);
      MIDI.sendControlChange(90+i,0,1);
    }
  }
}

// Use this function if you are using push buttons (momentary buttons)
void momentary_Buttons(){
  for(int i = 0; i < nButtons; i++){
    if (digitalRead(pinButtons[i]) == LOW) {
        MIDI.sendControlChange(90+i, 127, channel);
        flag[i] = 1;
    } else {
      if (flag[i] == 1 && digitalRead(pinButtons[i]) == HIGH) {
        MIDI.sendControlChange(90+i, 0, channel);
        flag[i] = 0;
      }   
    } 
  }
}


void ldr(){
  for(int i = 0; i < nPots; i++){
    value[i] = 127-(analogRead(potentiometer[i])/8); // 127 is to reverse the function of an LDR
    if(value[i] != lastvalue[i]){
      lastvalue[i] = value[i];
      MIDI.sendControlChange(1, value[i], channel);
      Serial.write(value[i]); 
    }
  }
  delay(10);
}

void potentiometers(){
  for(int i = 0; i < nPots; i++){
    //value[i] = analogRead(potentiometer[i])/8;
    //value[i] = constrain(value[i], 0, 127);

    value[i] = analogRead(potentiometer[i]);
    value[i] = map(value[i], 0, 1023, 0, 127);
    value[i] = constrain(value[i], 0, 127);
    
    if(value[i] != lastvalue[i]){
      lastvalue[i] = value[i];
      MIDI.sendControlChange(1, value[i], channel);
      Serial.write(value[i]); 
    }
  }
  delay(10);
}
