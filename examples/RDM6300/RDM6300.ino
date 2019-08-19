#include <RDM6300.h>

#define RFID_RX_PIN 13
#define RFID_TX_PIN 15

RDM6300 RDM(RFID_RX_PIN, RFID_TX_PIN);

int tag;

void setup() {
  Serial.begin(115200);
  Serial.println("Serial Ready");

}

void loop() {
  if(RDM.isIdUartAvailable()) {
    Serial.println(RDM.realTagString);
  }
  if(RDM.isIdAvailable()) {
    tag = RDM.readId();
    Serial.print("ID: ");
    Serial.println(tag,HEX);
  }

}
