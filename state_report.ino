
void state_report() {
  Serial.print(F("Current output: "));
   Serial.println(cOutput+1);
   Serial.print(F("Name: "));
  Serial.println(outputs[cOutput].Name);
   Serial.print(F("Active: "));
   Serial.println(outputs[cOutput].Active);
   Serial.print(F("IR protocol: "));
   Serial.println(outputs[cOutput].Protocol);
   Serial.print(F("IR command: 0x"));
   Serial.print(outputs[cOutput].vendorPrefixIR, HEX);
   Serial.println(outputs[cOutput].cmdIR, HEX);
   Serial.println();
   }
