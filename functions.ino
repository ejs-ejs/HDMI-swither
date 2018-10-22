// from https://arduino.stackexchange.com/questions/13165/how-to-read-pinmode-for-digital-pin
int pinMode(uint8_t pin)
{
  if (pin >= NUM_DIGITAL_PINS) return (-1);

  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *reg = portModeRegister(port);
  if (*reg & bit) return (OUTPUT);

  volatile uint8_t *out = portOutputRegister(port);
  return ((*out & bit) ? INPUT_PULLUP : INPUT);
}

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

void nextKey() {
  byte gotOutput = 0;
      while (!gotOutput) {
        cKey = (cKey == NUM_CMDS - 1) ? 0 : cKey + 1;
        if (outputs[cKey].Active) {
          gotOutput = 1;
        }
      }
  }



