void CMD_learn() {
  unsigned long irCodes[5];

  Serial.print(F("Press the key you want to assing to slot #"));
  Serial.println(cOutput + 1);

  unsigned int i = 0;
  // read the IR codes
  while (i < 5) {
    if (myReceiver.getResults()) {
      myDecoder.decode();           //Decode it
      if ( myDecoder.value && (myDecoder.value < 0xFFFFFFFF)) {
        if (cDevice.Debug) {
          Serial.print(F("IR: received signal "));
          Serial.print(F("[0x"));
          Serial.print(myDecoder.value, HEX);
          Serial.println(F("]"));

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
        if (i) {
          if (irCodes[i - 1] == myDecoder.value) {
            irCodes[i] = myDecoder.value;
          }
        } else {
          irCodes[i] = myDecoder.value;
        }
      }
      Serial.print(F("["));
      Serial.print(i);
      Serial.print(F("] "));
      i++;
    }
    myReceiver.enableIRIn();      //Restart receiver
  }

  // unsigned int cCmd = irCodes[0];
  // unsigned int cPrefix = irCodes[0] >> 16;
  // outputs[cOutput].vendorPrefixIR = cPrefix;
  // outputs[cOutput].cmdIR = cCmd;
  outputs[cOutput].vendorPrefixIR = irCodes[0] >> 16;
  outputs[cOutput].cmdIR = irCodes[0];
  outputs[cOutput].Protocol = myDecoder.protocolNum;

  state_report();
}

