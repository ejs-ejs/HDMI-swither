/* CONTROLL COMMANDS
    help command
*/

void CMD_help(void) {
Serial.println(F("\n\nThis is HDMI switcher"));
  Serial.print(F("\t version "));
  Serial.println(__VERSION);
  Serial.println();
  Serial.println("SELECT - select output for modification");
  Serial.println("SWITCH - swich to the selected input");
  Serial.println("STATE - print the state of the device");
  Serial.println();
  Serial.println("ENABLE - enable the selected output");
  Serial.println("DISABLE - disable the selected output");
  Serial.println("NAME - (re)name the selected output");
  Serial.println("LEARN - learn the IR command to the selected slot. Use with care");
  Serial.println();
  Serial.println("ejs <eugenijus.januskevicius@vgtu.lt>, 2018");
}
