#include <Wire.h>


#include "Adafruit_EEPROM_I2C.h"
#include "data.h"

/* Example code for the Adafruit I2C EEPROM breakout */

/* Connect SCL    to SCL
   Connect SDA    to SDA
   Connect VDD    to 3 - 5V DC
   Connect GROUND to common ground */
   
Adafruit_EEPROM_I2C i2ceeprom;

#define EEPROM_ADDR 0x50  // the default address!

void setup(void) {
  uint32_t max_addr;
  char buffer[16];
  uint16_t val;  
  int x;
  Serial.begin(115200);
  
  if (i2ceeprom.begin(0x50)) {  // you can stick the new i2c addr in here, e.g. begin(0x51);
    Serial.println("Found I2C EEPROM");
  } else {
    Serial.println("I2C EEPROM not identified ... check your connections?\r\n");
    //while (1) delay(10);
  }
  
  Serial.print("This EEPROM can store ");
  Serial.print(max_addr);
  Serial.println(" bytes");

#if 1
  x=0;
  max_addr= 2048;  
  for (uint16_t addr = 0; addr < max_addr; addr++) {
    val = pgm_read_word_near(&(full_src[x]));
    i2ceeprom.write(addr, (val >> 8) & 0xff);
    addr++;
    i2ceeprom.write(addr, val & 0xff);
    x++;
  }
 #endif


    
#if 1
  max_addr=4096; 
// dump the memory in HEX
  
  x=0;
  for (uint16_t addr = 0; addr < max_addr; addr++) {

    val = i2ceeprom.read(addr);
    val = ((val &0xff) <<8);
	addr++;
    val = val | (i2ceeprom.read(addr) &0xff);
    if(!(x%8)){
      memset(buffer, 0, sizeof(buffer));
      snprintf(buffer, sizeof(buffer), "\n");
      Serial.print(buffer);
    }
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "0x%04x,\t ",val);
    Serial.print(buffer);
	x++;
  }
#endif


#if 0
// DUMP the memory in ascii
  uint8_t val;
  int x=0;
  for (uint16_t addr = 0; addr < max_addr; addr++) {
    val = i2ceeprom.read(addr);
    if(!(x%8)){
      memset(buffer, 0, sizeof(buffer));
      snprintf(buffer, sizeof(buffer), "\n");
      Serial.print(buffer);
    }
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%c,\t ",val);
    Serial.print(buffer);
	x++;
  }
#endif  
}

void loop(void) {

}
