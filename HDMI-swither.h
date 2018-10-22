
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


char __VERSION[] = "0.1b"; // version


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
  // char Protocol[9]; // IR protocol
  byte Protocol; // IR protocol
} Output;

