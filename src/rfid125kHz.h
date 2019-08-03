/*
RFID reader.
Hardware: RDM6300 or RF125-PS or Gwiot 7941E
Uses 125KHz RFID tags.
*/

#ifndef rfid125kHz_h
#define rfid125kHz_h

#include "Arduino.h"

class RFID_Reader
{
public:
    void rfidSerial(char x);
    bool Available();
    String GetHexID();
    String GetDecID();
    String GetTagType();
private:
    char *ulltostr(unsigned long long value, char *ptr, int base);
    void parse();
    uint8_t char2int(char c);
    uint8_t get_checksum(unsigned long long data);
    static const char asciiNum_diff = 48;
    static const char asciiUpp_diff = 7;
    bool data_available = false;
    unsigned long long new_ID = 0ULL;
    unsigned long long last_ID = 0ULL;
    uint8_t tagtype;
    uint8_t lasttagtype;
    unsigned long LastRFID = 0UL;
    char msg[15];
    uint8_t msgLen;
    byte ix = 0;
    byte StartByte = 0x02;
    byte EndByte = 0x03;
    typedef struct { 
        uint8_t itype;
        char* stype;
    } typeDictionary;

    const typeDictionary typeDict[12] {
        {0x01, (char*)"MIFARE 1K"},
        {0x02, (char*)"EM4100"},
        {0x03, (char*)"MIFARE 4K"},
        {0x10, (char*)"HID card"},
        {0x11, (char*)"T5567"},
        {0x20, (char*)"2G certificate"},
        {0x21, (char*)"IS014443B"},
        {0x22, (char*)"FELICA"},
        {0x30, (char*)"15693 tag"},
        {0x50, (char*)"CPU card"},
        {0x51, (char*)"sector information"},
        {0xFF, (char*)"keyboard data"}
    };   
};
#endif
