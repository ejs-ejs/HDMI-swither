void CMD_state() {
  Serial.println(F("\n\nThis is HDMI switcher"));
  Serial.print(F("\t version "));
  Serial.println(__VERSION);
  Serial.println(F("=============================="));
  Serial.print(F("Location: "));
  Serial.println(cDevice.Location);
  Serial.print(F("debounce time = "));
  Serial.print(cDevice.tDebounce, DEC);
  Serial.println(F(" ms"));
  Serial.print(F("Debug:\t"));
  cDevice.Debug?Serial.println(F("yes")):Serial.println(F("no"));;

Serial.println(F("Output \t Name \t Active \t Proto\t Vend. pref. \t IR cmd"));
  
  for ( unsigned int i=0;i<NUM_CMDS; i++) {
   Serial.print(i+1);
   Serial.print(F("\t"));
    char buff[17];
    sprintf(buff,"%s",outputs[i].Name);
  Serial.print(buff);
  
   //Serial.print(outputs[i].Name);
   Serial.print(F("\t"));
   outputs[i].Active?Serial.print(F("Y")):Serial.print(F("N"));
   Serial.print(F("\t"));
   Serial.print(outputs[i].Protocol);
   Serial.print(F("\t0x"));
   Serial.print(outputs[i].vendorPrefixIR, HEX);
   Serial.print(F("\t"));
   Serial.println(outputs[i].cmdIR, HEX);
    }
  
  
}
