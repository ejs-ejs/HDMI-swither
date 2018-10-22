void CMD_switch() {
  cKey = cOutput;

  infoLED(1);

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

  nextKey();
  infoLED(0);
  state_report();
}
