/*
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino       Arduino     Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Teensy3.1      ESP32      ESP8266
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         9                RST       RST
 * SPI SS      SDA(SS)      10            53        D10        10               15        GPIO-15 | D8 
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        11               13        GPIO-13 | D7 
 * SPI MISO    MISO         12 / ICSP-1   50        D12        12               12        GPIO-12 | D6  
 * SPI SCK     SCK          13 / ICSP-3   52        D13        13               14        GPIO-14 | D5  
 *
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         5         
#define SS_PIN          15        

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
	Serial.begin(115200);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Dump debug info about the card; PICC_HaltA() is automatically called
	mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}
