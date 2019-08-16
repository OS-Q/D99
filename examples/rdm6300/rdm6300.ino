#include <SoftwareSerial.h>
#include <RDM6300.h>

#define RFID_RX_PIN 5
#define RFID_TX_PIN 4

RDM6300 RDM(RFID_RX_PIN, RFID_TX_PIN);

int tag;

void setup() {
  Serial.begin(115200);
  Serial.println("Serial Ready");

}

void loop() {
  if(RDM.isIdUartAvailable()) {
    //tag = RDM.readId();
    Serial.print("isIdUartAvailable");
    Serial.println(RDM.realTagString);
  }
  if(RDM.isIdAvailable()) {
    tag = RDM.readId();
    Serial.print("ID: ");
    Serial.println(tag,HEX);
  }

}
