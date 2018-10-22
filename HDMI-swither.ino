#include <LiquidCrystal_PCF8574.h>
#include <Bounce2.h>
#define T_DEBOUNCE 200


// command inteface
#include <SerialCommand.h>

#include "key_digital.h"

//This includes everything. Not generally recommended.
//#include <IRLibAll.h>

// collect the IRLib2 stack
#include <IRLibDecodeBase.h>  //We need both the coding and sending

// include the send base
#include <IRLibSendBase.h>

//Now include only the protocols you wish to actually use.
//The lowest numbered protocol should be first but remainder
//can be any order.
#include <IRLib_P01_NEC.h>

// Include a receiver either this or IRLibRecvPCI or IRLibRecvLoop
#include <IRLibRecvPCI.h>


#include <IRLibCombo.h>     // After all protocols, include this
// All of the above automatically creates a universal sending
// class called "IRsend" containing only the protocols you want.
// Now declare an instance of that sender.

//#include <IRLibAll.h>

#include <Wire.h>

#include "HDMI-swither.h"

IRsend mySender;

SerialCommand sCmd;     // The SerialCommand object

LiquidCrystal_PCF8574 lcd(I2C_LCD_ADDR);

Device cDevice;

IRrecvPCI myReceiver(IRRECEIVER); //create receiver and pass pin number

IRdecode myDecoder;   //create decoder

// Instantiate a Bounce object
Bounce debouncer[NUM_KEYS] = Bounce();

//
//  global variables
//
 
short haveLCD = 0;
unsigned int key = 0;
unsigned long pattern;
byte stateMachine;
unsigned long tRunning, tWake;
Output outputs[NUM_CMDS];
unsigned int cKey = 0;
unsigned int cOutput = 0; // current output for command interface

// define the physical connection of buttons to the pins in the array
// Keys[0] is the button #1
unsigned int Keys[] = {12, 9, 10, 11};

unsigned long previousMillis = 0;        // will store last time push button occured

//
// main 
//
void loop() {

// read serial port
  sCmd.readSerial();
  
  // update buttons
  buttonState();
  // Serial.print(F(" DEBUG: StateMachine: "));
  //  Serial.println(stateMachine, BIN);
  //  Serial.print(F(" DEBUG: cKey: "));
  //  Serial.println(cKey);

  if (stateMachine & 1) { // 1st key cycle sisft inputs
    infoLED(1);
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= (cDevice.tDebounce << 3)) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      // do al lthe dirty job


      pattern = long(outputs[cKey].vendorPrefixIR) << 16 | outputs[cKey].cmdIR;
      mySender.send(outputs[cKey].Protocol, pattern, 32); // send info to beamer
   
      
      if (cDevice.Debug) {
        Serial.print(F("Switching input #"));
        Serial.print(cKey + 1);
        Serial.print(F(" - "));
        Serial.print(outputs[cKey].Name);
        Serial.print(F(" . Signal sent: 0x"));
        Serial.println(pattern, HEX);
      }
      if (haveLCD) {
        reportLCD();
      }
      stateMachine = stateMachine ^ 1; // clear the 1st bit
      nextKey();
      infoLED(0);
    }
  }
}
