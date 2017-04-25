#include "Arduino.h"
#include <Wire.h>
#define DEVID 0x50

void writeByte(unsigned long address, byte device, byte wdata); // address 0x00000 - 0x1FFFF, hence long type
byte readByte(unsigned long address, byte device);              // device only needs 1 byte for the address, usually 0x50

void setup()
{
   Serial.begin(9600);
   Serial.println("Begin!");

   unsigned long address = 0x0FFFF;

   Wire.begin();

      //Serial.println("Writing 240 to address 2");
   writeByte(address, DEVID, 240);  // writes "240" to address in EEPROM
   Serial.println(readByte(address, DEVID), HEX);
   Serial.println("END!");
}

void loop()
{

}


void writeByte(unsigned long address, byte device, byte wdata)
{
   device += (address >> 16);    // these instructions adjust the right block for the EEPROM
   address &= 0xFFFF;

    Wire.beginTransmission(device); // send address
    Wire.write((address >> 8));     // MSB
    Wire.write((address & 0xFF));   // LSB
    Wire.write(wdata);              // data
    Wire.endTransmission();

    delay(10);                  // delay 10 ms to end write sequence
}

byte readByte(unsigned long address, byte device)
{
   byte rdata;

   device += (address >> 16);   // these instructions adjust the right block for the EEPROM
   address &= 0xFFFF;

   Wire.beginTransmission(device);
   Wire.write((address >> 8));
   Wire.write((address & 0xFF));
   Wire.endTransmission();
   Wire.requestFrom(device,1);

   if (Wire.available())
   {
       rdata = Wire.read();
   }

   return rdata;
}
