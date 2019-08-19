#include "SPI.h"
#include "NDEF.h"

PN532_SPI pn532spi(SPI, 10);
SNEP nfc(pn532spi);
uint8_t ndefBuf[128];

void setup() {
    Serial.begin(9600);
    Serial.println("NFC Peer to Peer Example - Send Message");
}

void loop() {
    Serial.println("Send a message to Peer");
    
    NdefMessage message = NdefMessage();
    message.addUriRecord("http://shop.tfzoo.com/product/mobile/06.do");
    //message.addUriRecord("http://arduino.cc");
    //message.addUriRecord("https://github.com/OS-Q/D104");

    
    int messageSize = message.getEncodedSize();
    if (messageSize > sizeof(ndefBuf)) {
        Serial.println("ndefBuf is too small");
        while (1) {
        }
    }

    message.encode(ndefBuf);
    if (0 >= nfc.write(ndefBuf, messageSize)) {
        Serial.println("Failed");
    } else {
        Serial.println("Success");
    }

    delay(3000);
}
