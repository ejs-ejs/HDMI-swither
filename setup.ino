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
    Serial.print(F("Pin 3 is "));
    pinMode(IRLED)?Serial.println("OUTPUT"):Serial.println("INPUT");
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
  //strcpy(outputs[0].Protocol, NEC);
 outputs[0].Protocol=NEC;

  outputs[1].Active = 1;
  strcpy(outputs[1].Name, "HDMI");
  outputs[1].cmdIR = KEY_DIGITAL_INPUT_2;
  outputs[1].vendorPrefixIR = KEY_DIGITAL_PREFIX;
  //strcpy(outputs[1].Protocol, NEC);
  outputs[1].Protocol=NEC;

  outputs[2].Active = 1;
  strcpy(outputs[2].Name, "VGA");
  outputs[2].cmdIR = KEY_DIGITAL_INPUT_3;
  outputs[2].vendorPrefixIR = KEY_DIGITAL_PREFIX;
  //strcpy(outputs[2].Protocol, NEC);
  outputs[2].Protocol=NEC;


  outputs[3].Active = 0;
  strcpy(outputs[3].Name, "N/A");
  outputs[3].cmdIR = KEY_DIGITAL_INPUT_4;
  outputs[3].vendorPrefixIR = KEY_DIGITAL_PREFIX;
  //strcpy(outputs[3].Protocol, NEC);
  outputs[3].Protocol=NEC;


  pattern = long(outputs[cKey].vendorPrefixIR) << 16 | outputs[cKey].cmdIR;
  mySender.send(outputs[cKey].Protocol, pattern, 32); // send info to beamer
 
  if (haveLCD) {
    reportLCD();
  }
  nextKey();

  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("SELECT",    CMD_output_select);          // select output for manipulations
  sCmd.addCommand("ENABLE",   CMD_output_enable);         // enable output
  sCmd.addCommand("DISABLE",   CMD_output_disable);         // disable output
  sCmd.addCommand("NAME",   CMD_output_name);         // name output
  sCmd.addCommand("STATE",   CMD_state);         // print state to serial port
  sCmd.addCommand("LEARN",   CMD_learn);         // learn the IR code rom print state to serial port
  sCmd.addCommand("SWITCH",   CMD_switch);         // switch to teh selected input
  sCmd.addCommand("?", CMD_help);        // help un use
  sCmd.setDefaultHandler(CMD_help);      // Handler for command that isn't matched


}
