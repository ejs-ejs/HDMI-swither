#include <LiquidCrystal_PCF8574.h>
#include <Bounce2.h>
#define T_DEBOUNCE 200

#include "key_digital.h"

//This includes everything. Not generally recommended.
#include <IRLibAll.h>

// collect the IRLib2 stack
#include <IRLibDecodeBase.h>  //We need both the coding and sending

// include the send base
#include <IRLibSendBase.h>

//Now include only the protocols you wish to actually use.
//The lowest numbered protocol should be first but remainder
//can be any order.
#include <IRLib_P01_NEC.h>

// Include a receiver either this or IRLibRecvPCI or IRLibRecvLoop
#include <IRLibRecv.h>

#include <IRLibCombo.h>     // After all protocols, include this
// All of the above automatically creates a universal sending
// class called "IRsend" containing only the protocols you want.
// Now declare an instance of that sender.

#include <Wire.h>


IRsend mySender;

//#define I2C_LCD_ADDR 0x20
//#define I2C_LCD_ADDR 0x27
//#define I2C_LCD_ADDR 0x38
#define I2C_LCD_ADDR 0x3F

#define LCD_COLS 16
#define LCD_ROWS 16

#define NUM_KEYS 1
#define NUM_CMDS 4

#define IRRECEIVER 2
#define IRLED 3
#define RLED 13


typedef struct {
  char Location[17]; // location of the device
  unsigned int tDebounce; // debounce time, ms
  unsigned int vendorPrefix; // vendor prefix of the output
  byte Debug;
} Device;

typedef struct {
  byte Active;
  char Name[17]; // name of the output
  char cmdRS232[13]; // RS-232 command to activate
  unsigned int cmdIR; // IR command to activate
  unsigned int vendorPrefixIR; // IR vendor prefix of the output
  char Protocol[9]; // IR protocol
} Output;

LiquidCrystal_PCF8574 lcd(I2C_LCD_ADDR);
Device cDevice;
IRrecv myReceiver(IRRECEIVER); //create receiver and pass pin number
IRdecode myDecoder;   //create decoder

short haveLCD = 0;

// Instantiate a Bounce object
Bounce debouncer[NUM_KEYS] = Bounce();

unsigned int key = 0;
unsigned long pattern;

byte stateMachine;
unsigned long tRunning, tWake;
Output outputs[NUM_CMDS];

unsigned int cKey = 0;

// define the physical connection of buttons to the pins in the array
// Keys[0] is the button #1
unsigned int Keys[] = {12, 9, 10, 11};


unsigned long previousMillis = 0;        // will store last time push button occured



/* update button states
    store them in the global variable
*/
void buttonState() {
  for ( byte n = 0; n < NUM_KEYS; n++) {
    // Update the Bounce instance :
    debouncer[n].update();
    // Get the updated value :
    if (debouncer[n].fell()) {
      stateMachine = stateMachine | _BV(n); // set the Nth bit
    }
  }
}


void infoLED( byte state) {
  digitalWrite(RLED, state);
  stateMachine ^= state = 1 ? 1 << 6 : 0 << 6; // will it work as expected?
}

void reportLCD() {
  lcd.clear();
  lcd.setCursor(0, 0); //we start writing from the first row first column
  lcd.print(cDevice.Location);

  lcd.setCursor(0, 1);
  lcd.print("[#");
  lcd.print(cKey + 1);
  lcd.print("] ");
  lcd.print(outputs[cKey].Name);
}


void setup() {
  int error;
  strcpy(cDevice.Location, "VGTU AF 2.12 aud");
  cDevice.tDebounce = 100;
  cDevice.Debug = 1;
  Serial.begin(115200);
  if (cDevice.Debug) {
    Serial.println(F("Attaching buttons:"));
  }
  for ( byte n = 0; n < NUM_KEYS; n++) {
    pinMode(Keys[n], INPUT_PULLUP);
    debouncer[n].attach(Keys[n]);
    debouncer[n].interval(cDevice.tDebounce);
    if (cDevice.Debug) {
      Serial.print(F("\t#"));
      Serial.print(n, DEC);
      Serial.print(" to ");
      Serial.println(Keys[n], DEC);
    }
  }



  delay(200); while (!Serial);
  myReceiver.enableIRIn(); // Start the receiver

  Serial.println(F("Ready to receive IR signals"));

  if (cDevice.Debug) {
    Serial.print(F("Checking for LCD on 0x"));
    Serial.println(I2C_LCD_ADDR, HEX);
  }

  // See http://playground.arduino.cc/Main/I2cScanner
  Wire.begin();
  Wire.beginTransmission(I2C_LCD_ADDR);
  error = Wire.endTransmission();

  if (error == 0) {
    Serial.print(F("LCD OK, found at 0x"));
    Serial.println(I2C_LCD_ADDR, HEX);
    lcd.begin(LCD_COLS, LCD_ROWS); // initialize the lcd
    lcd.setBacklight(255);
    lcd.clear();
    lcd.home();
    lcd.setCursor(0, 0); //we start writing from the first row first column
    lcd.print(F("Ready to receive IR signals"));
    haveLCD = 1;

  } else {
    Serial.print(F("ERROR: LCD not found at 0x"));
    Serial.println(I2C_LCD_ADDR, HEX);
  } // if

  outputs[0].Active = 1;
  strcpy(outputs[0].Name, "Computer");
  outputs[0].cmdIR = KEY_DIGITAL_INPUT_1;
  outputs[0].vendorPrefixIR = KEY_DIGITAL_PREFIX;
  strcpy(outputs[0].Protocol, "NEC");

  outputs[1].Active = 1;
  strcpy(outputs[1].Name, "HDMI");
  outputs[1].cmdIR = KEY_DIGITAL_INPUT_2;
  outputs[1].vendorPrefixIR = KEY_DIGITAL_PREFIX;
  strcpy(outputs[1].Protocol, "NEC");

  outputs[2].Active = 1;
  strcpy(outputs[2].Name, "VGA");
  outputs[2].cmdIR = KEY_DIGITAL_INPUT_3;
  outputs[2].vendorPrefixIR = KEY_DIGITAL_PREFIX;
  strcpy(outputs[2].Protocol, "NEC");

  outputs[3].Active = 0;
  strcpy(outputs[3].Name, "N/A");
  outputs[3].cmdIR = KEY_DIGITAL_INPUT_4;
  outputs[3].vendorPrefixIR = KEY_DIGITAL_PREFIX;
  strcpy(outputs[3].Protocol, "NEC");

  pattern = long(outputs[cKey].vendorPrefixIR) << 16 | outputs[cKey].cmdIR;
  mySender.send(outputs[cKey].Protocol, pattern, 32); // send info to beamer

  if (haveLCD) {
    reportLCD();
  }

}

void loop() {

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
      byte gotCOutput = 0;
      while (!gotCOutput) {
        cKey = (cKey == NUM_CMDS - 1) ? 0 : cKey + 1;
        if (outputs[cKey].Active) {
          gotCOutput = 1;
        }
      }
      infoLED(0);
    }
  }


  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
    if ( myDecoder.value && (myDecoder.value < 0xFFFFFFFF)) {
      if (cDevice.Debug) {
        Serial.print(F("IR: received signal "));
        Serial.println(myDecoder.value);
        //myDecoder.dumpResults(true);  //Now print results. Use false for less detail
        myDecoder.dumpResults(false);

        if (haveLCD) {
          lcd.clear();//Clean the screen
          lcd.setCursor(1, 0);
          lcd.print(F("Received:"));
          lcd.setCursor(0, 1);
          lcd.print(F("[0x"));
          lcd.print(myDecoder.value, HEX);
          lcd.print(F("]"));
        }
      }

    }
    myReceiver.enableIRIn();      //Restart receiver
  }
}
