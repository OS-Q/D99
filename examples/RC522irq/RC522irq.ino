#include <SPI.h>
#include <MFRC522.h>
//#include <String.h>

#define RST_PIN     5
#define SS_PIN      15

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
volatile bool cardPresent;
void ICACHE_RAM_ATTR isr()
{
  cardPresent = true;
}
void setup()
{
  Serial.begin(115200);       // Initialize serial communications with the PC
  while (!Serial);      // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();       // Init MFRC522
  mfrc522.PCD_WriteRegister(MFRC522::ComIrqReg, 0x80); //Clear interrupts
  mfrc522.PCD_WriteRegister(MFRC522::ComIEnReg, 0x7F); //Enable all interrupts
  mfrc522.PCD_WriteRegister(MFRC522::DivIEnReg, 0X84);
  mfrc522.PCD_WriteRegister(MFRC522::DivIEnReg, 0x84); //
  mfrc522.PCD_WriteRegister(MFRC522::DivIrqReg, 0x80); //
  mfrc522.PCD_WriteRegister(MFRC522::ComIEnReg, 0xA0); //
  mfrc522.PCD_WriteRegister(MFRC522::ComIrqReg, 0x80);
  mfrc522.PCD_WriteRegister(MFRC522::ComIrqReg, 0x80); //
//  mfrc522.PCD_WriteRegister(MFRC522::FIFODataReg, PICC_REQIDL);
//  mfrc522.PCD_WriteRegister(MFRC522::CommandReg, PCD_TRANSCEIVE);
  mfrc522.PCD_WriteRegister(MFRC522::BitFramingReg, 0x87);
  Serial.println(F("Ready..."));
  /* setup the IRQ pin*/
  pinMode(4, INPUT);
  attachInterrupt(4, isr, FALLING);
}
void loop()
{
  if (cardPresent)
  {
    Serial.println(F("Interrupt"));
    mfrc522.PCD_WriteRegister(MFRC522::ComIrqReg, 0x7f); //Clear interrupts
    cardPresent = false;
  }

}
