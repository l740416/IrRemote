/*
    Copyright 2019 Chiehting Huang <j.huang.1985@gmail.com>
*/
#include "Arduino.h"
#include "IrRemote_I2C.h"

IrRemote_I2C::IrRemote_I2C(uint8_t i2cAddress) {
  _exist = false;
  _address = i2cAddress;
}

#if defined(ARDUINO_ARCH_ESP8266)
bool IrRemote_I2C::begin(int SDA_pin, int SCL_pin) {
    Wire.begin(SDA_pin,SCL_pin);
#else
bool IrRemote_I2C::begin() {
    Wire.begin();
#endif
    Wire.beginTransmission(_address);
    if (Wire.endTransmission() == 0) {
        _exist = true;
    }
    return _exist;
}

bool IrRemote_I2C::checkExist()
{
  Wire.beginTransmission(_address);
  if (Wire.endTransmission() == 0) {
      _exist = true;
  }
  return _exist;
}

bool IrRemote_I2C::exist() 
{
    return _exist;
}

void IrRemote_I2C::SetData(uint32 type, uint32 value, uint32 len)
{
  write32(CMD_SET_CODETYPE, type);
  write32(CMD_SET_CODEVALUE, value);
  write32(CMD_SET_CODELENGTH, len);
}

void IrRemote_I2C::SetRawData(uint16 kHz, uint16 len, uint16* data)
{
  uint32_t v = kHz | len << 16;
  write32(CMD_WRITE_KHZ_LEN, v);

  for (uint16_t i = 0; i < len; i += 2)
  {
    write32(CMD_WRITE_BUFFER + (i >> 1), *(uint32_t*)(data + i));  
  }
}

void IrRemote_I2C::SetRawkHz(uint16 kHz)
{
  uint32_t v = read32(CMD_GET_BUFFER_KHZ_LEN);
  v = (v & 0xFFFF00) | (kHz & 0xFFFF);
  write32(CMD_WRITE_KHZ_LEN, v);
}

uint32_t IrRemote_I2C::read32(uint8_t reg)
{
		uint32_t value = 0;
    uint8_t* data = (uint8_t*)&value;

    Wire.beginTransmission(_address);
#if ARDUINO >= 100
    Wire.write(reg);
#else
    Wire.send(reg);
#endif
    Wire.endTransmission();

    Wire.requestFrom(_address, (uint8_t) 4);
#if ARDUINO >= 100
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
#else
    data[0] = Wire.receive();
    data[1] = Wire.receive();
    data[2] = Wire.receive();
    data[3] = Wire.receive();
#endif

    return value;
}

void IrRemote_I2C::write0(uint8_t reg) {
	Wire.beginTransmission(_address);
	
#if (ARDUINO >= 100)
    Wire.write(reg);
#else
    Wire.send(reg);
#endif

    Wire.endTransmission();	
}

void IrRemote_I2C::write16(uint8_t reg, uint16_t data) {
    Wire.beginTransmission(_address); // start transmission to device 

    uint8_t x1, x2;
        
    x1 = (data >>  0 ) & 0xFF;
    x2 = (data >>  8 ) & 0xFF;
    
#if (ARDUINO >= 100)
    Wire.write(reg); // sends register address to read from
    Wire.write(x1);  // write data
    Wire.write(x2);  // write data
#else
    Wire.send(reg); // sends register address to read from
    Wire.send(x1);  // write data
    Wire.send(x2);  // write data
#endif

    Wire.endTransmission();
}

void IrRemote_I2C::write32(uint8_t reg, uint32_t data) {
    Wire.beginTransmission(_address); // start transmission to device 

    uint8_t x1, x2, x3, x4;
        
    x1 = (data >>  0 ) & 0xFF;
    x2 = (data >>  8 ) & 0xFF;
    x3 = (data >> 16 ) & 0xFF;
    x4 = (data >> 24 ) & 0xFF;
    
    //Serial.printf("[%02x][%02x][%02x][%02x]\n", x1, x2, x3, x4);
    
#if (ARDUINO >= 100)
    Wire.write(reg); // sends register address to read from
    Wire.write(x1);  // write data
    Wire.write(x2);  // write data
    Wire.write(x3);  // write data
    Wire.write(x4);  // write data
#else
    Wire.send(reg); // sends register address to read from
    Wire.send(x1);  // write data
    Wire.send(x2);  // write data
    Wire.send(x3);  // write data
    Wire.send(x4);  // write data
#endif

    Wire.endTransmission();
}



