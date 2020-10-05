# HDMI-swither
Arduino based remote controll for concealed SoHo infrared controlled input swithers. It controls Key Digital 4x1 HDMI swicher via infrared eye by default. Active output is displayed in the 16x2 I²C LCD screen. It can controll any IR equipment, if the protocol is supported by IR library and the controll valueas are provided via the H file.
The LCD displays the name of the current output (configurable).
The button will cycle the outputs. 

The device is confugurable via the serial interface. You can rename the outputs and set any of them to be skiped.
Valuea are stored in the flash memory of the uC.
## Commands
  SELECT - select output for manipulations
  ENABLE - enable the selected output
  DISABLE - disable the selected output
  NAME - rename the selected output
  STATE - print state to serial port
  LEARN - read the IR code (the receiver must be connected) and print it to the serial port
  SWITCH - switch to the selected input
  ? - help on use
  
