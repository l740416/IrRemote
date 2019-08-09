/*
  Copyright 2019 Chiehting Huang <j.huang.1985@gmail.com>

*/
#ifndef IRREMOTE_I2C_h
#define IRREMOTE_I2C_h

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
 #include "TinyWireM.h"
 #define Wire TinyWireM
#elif defined(ARDUINO_ARCH_ESP8266)
 #include <Wire.h>
#else
 #include <avr/pgmspace.h>
 #include <Wire.h>
#endif

// I2C commands
#define SLAVE_ADDRESS                  0x05
#define MAX_SENT_BYTES                 5
#define CMD_INFO                       0x00   // Out:4B
#define CMD_GET_CODETYPE               0x01   // Out:4B
#define CMD_GET_CODEVALUE              0x02   // Out:4B
#define CMD_GET_CODELENGTH             0x03   // Out:4B
#define CMD_GET_BUFFER_KHZ_LEN         0x04
#define CMD_GET_BUFFER                 0x05

#define CMD_SET_CODETYPE               0x80   // In :4B
#define CMD_SET_CODEVALUE              0x81   // In :4B
#define CMD_SET_CODELENGTH             0x82   // In :4B

#define CMD_RECV                       0x90
#define CMD_SEND                       0x91
#define CMD_SEND_RAW                   0x92
#define CMD_STORE_RAW                  0x93
#define CMD_STOP_STORE_RAW             0x94

#define CMD_WRITE_KHZ_LEN              0xA0
#define CMD_WRITE_BUFFER               0xA1

class IrRemote_I2C
{
  public:
    IrRemote_I2C(uint8_t i2cAddress = SLAVE_ADDRESS);
#if defined(ARDUINO_ARCH_ESP8266)
    bool begin(int SDA_pin, int SCL_pin);
#else
    bool begin();
#endif
    bool checkExist();
    bool exist();
    uint32_t Info()                     { return read32(CMD_INFO); }
    uint8_t  MajorVer(uint32_t info)    { return (info >>  4) & 0x0F;}
    uint8_t  MinorVer(uint32_t info)    { return  info        & 0x0F;}
    uint16_t BufferSize(uint32_t info)  { return (info >>  8) & 0xFF;}
    uint16_t RecvCount(uint32_t info)   { return (info >> 16) & 0xFF;}
    
    uint32_t GetCodeType()              { return read32(CMD_GET_CODETYPE); }
    uint32_t GetCodeValue()             { return read32(CMD_GET_CODEVALUE); }
    uint32_t GetCodeLength()            { return read32(CMD_GET_CODELENGTH); }

    uint16_t GetRawCodekHz()            { return read32(CMD_GET_BUFFER_KHZ_LEN) & 0xFF; }
    uint16_t GetRawCodeLength()         { return read32(CMD_GET_BUFFER_KHZ_LEN) >> 16; }
    
    // Return 2 data a time (2 x uint16_t)
    uint32_t GetRawData(uint16_t index) { return read32(CMD_GET_BUFFER + index); }

    void Send()      {write0(CMD_SEND);}            ///< Send using known code type/value/length
    void SendRaw()   {write0(CMD_SEND_RAW);}        ///< Send using raw data
    void StoreRaw()  {write0(CMD_STORE_RAW);}       ///< Store received code into raw data
    void StopStore() {write0(CMD_STOP_STORE_RAW);}  ///< Abort store command
    void Recv()      {write0(CMD_RECV);}            ///< Start Receive (no need?)
    
    void SetData(uint32 type, uint32 value, uint32 len);
    void SetRawkHz(uint16 kHz);
    void SetRawData(uint16 kHz, uint16 len, uint16* data);
    

  private:
    void     write0(uint8_t reg);
    void     write16(uint8_t reg, uint16_t data);
    void     write32(uint8_t reg, uint32_t data);
    uint32_t read32(uint8_t reg);
    
    bool    _exist;
    uint8_t _address;
};

#endif

