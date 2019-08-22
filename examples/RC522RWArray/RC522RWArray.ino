/**
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino     Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro   ESP8266
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST       RST
 * SPI SS      SDA(SS)      10            53        D10        10               10        GPIO-15 | D8 
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16        GPIO-13 | D7 
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14        GPIO-12 | D6  
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15        GPIO-14 | D5  
 * IRQ         ?            ?             ?         ?          2                10         GPIO-4 | D2 
 * 
 */

/*
writeBlock(blockNumber,arrayAddress);

void writeBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;
  mfrc522PCD_Authenticate(rfid::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522uid));
  mfrc522MIFARE_Write(blockNumber, arrayAddress, 16);
  //mfrc522MIFARE_Write(9, value1Block, 16);
}



if (mfrc522isCard()) {
        if (mfrc522readCardSerial()) {
            if (mfrc522serNum[0] != serNum0
                || mfrc522serNum[1] != serNum1
                || mfrc522serNum[2] != serNum2
                || mfrc522serNum[3] != serNum3
                || mfrc522serNum[4] != serNum4
            ) {
                // With a new cardnumber, show it.
                Serial.println(" ");
                Serial.println("Card found");
                serNum0 = mfrc522serNum[0];
                serNum1 = mfrc522serNum[1];
                serNum2 = mfrc522serNum[2];
                serNum3 = mfrc522serNum[3];
                serNum4 = mfrc522serNum[4];
               
                //Serial.println(" ");
                Serial.println("Cardnumber:");
                Serial.print("Dec: ");
		Serial.print(mfrc522serNum[0],DEC);
                Serial.print(", ");
		Serial.print(mfrc522serNum[1],DEC);
                Serial.print(", ");
		Serial.print(mfrc522serNum[2],DEC);
                Serial.print(", ");
		Serial.print(mfrc522serNum[3],DEC);
                Serial.print(", ");
		Serial.print(mfrc522serNum[4],DEC);
                Serial.println(" ");



byte status = mfrc522MIFARE_Write(blockAddr, valueBlock, 16);
if (status != rfid::STATUS_OK) {
	Serial.print("MIFARE_Write() failed: ");
	Serial.println(mfrc522GetStatusCodeName(status));
}
*/

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          15          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

    // In this sample we use the second sector,
    char PK_ID[2]         = {'A', 'B'};
    byte RFID_ID[4];
    byte TIME[6]          = {16, 10, 23, 19, 47, 23};
    
    // PK_ID[2], RFID_ID[4], TIME[6], 0000
    byte DATA_HTTP [16];
    // PK_ID[2], TIME[6], 00000000
    byte DATA_RFID [16];
 /*   
    byte RFID_1K [SECTOR][SECTOR][TRAILER] = {
                                             {  
                                               {4, 5, 6}  
                                            
                                            
                                            }   
 */    
    
    byte sector[15]    = { 1,  2,  3,    4,  5,  6,    7,  8,  9,   10, 11, 12,   13, 14, 15 };
    byte blockAddr[45] = { 4,  5,  6,    8,  9, 10,   12, 13, 14,   16, 17, 18,   20, 21, 22,   
                          24, 25, 26,   28, 29, 30,   32, 33, 34,   36, 37, 38,   40, 41, 42,   
                          44, 45, 46,   48, 49, 50,   52, 53, 54,   56, 57, 58,   60, 61, 62 };
        
    byte trailer[15]  = { 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63 };
    byte trailerBlock = trailer[0];
    
    boolean flag_rfid = true;
    boolean flag_rfid_auth_a = true;
    boolean flag_rfid_auth_b = true;    
    boolean flag_rfid_read = true;
    boolean flag_rfid_write = true;
    
    
    byte buffer[18];
    byte size = sizeof(buffer);



/**
 * Initialize.
 */
void setup() {
    Serial.begin(115200); // Initialize serial communications with the PC
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xff;
    }

    Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
    Serial.print(F("Using key (for A and B):"));
    debug_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();
    
    Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));

}



//*******************************************************************************//
//*******************************************************************************//
//******************************* Start Main Loop *******************************//
//*******************************************************************************//
//*******************************************************************************//
void loop() {
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
    
  /*  clear_rfid ();
    // Dump debug info about the card; PICC_HaltA() is automatically called    
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    delay (5000);*/
   
    // Store RFID_ID into array
    for (byte i = 0; i < 4; i++) {
      RFID_ID[i] = mfrc522.uid.uidByte[i];
    }
    
    create_data_http(RFID_ID, TIME);
    create_data_rfid(TIME);
    
    /* // Call debug function - Remove for future purpose   
    Serial.println(F("RFID ID:"));
    debug_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    // Call debug function - Remove for future purpose */
    
    
    /* Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // Check for compatibility
    if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("This sample only works with MIFARE Classic cards."));
        return;
    } */

    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK){
          flag_rfid = false;
          return;
      }else flag_rfid = true;


    /* // Authenticate using key A
    Serial.println(F("Authenticating again using key A..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    } */

    /* // Show the whole sector as it currently is
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector[0]);
    Serial.println(); */

    /* // Read data from the block
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr[0], buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }*/
    
    
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr[2], buffer, &size);
      if (status != MFRC522::STATUS_OK) {
          flag_rfid = false;
          return;
      }else flag_rfid = true;
    
    
    
    // Serial.print(F("Data in block ")); Serial.print(blockAddr[0]); Serial.println(F(":"));
    debug_byte_array(buffer, 16); Serial.println();
    Serial.println();

    /* // Authenticate using key B
    Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    } */
    
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK){
          flag_rfid = false;
          return;
      }else flag_rfid = true;
      

    // Write data to the block
    // Serial.print(F("Writing data into block ")); Serial.print(blockAddr[0]);
    debug_byte_array(DATA_RFID, 16); Serial.println();
    
    /*status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr[0], DATA_RFID, 16);
    // status = mfrc522.MIFARE_Write(blockAddr[0], DATA_RFID, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    } */
    
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr[2], DATA_RFID, 16);
      if (status != MFRC522::STATUS_OK) {
          flag_rfid = false;
          return;
      }else flag_rfid = true;
    


    // Read data from the block (again, should now be what we have written)
    // Serial.print(F("Reading data from block ")); Serial.print(blockAddr[0]);
    /* Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr[0], buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    } */
    
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr[2], buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        flag_rfid = false;
        return;
    }else flag_rfid = true;
    
    // Serial.print(F("Data in block ")); Serial.print(blockAddr[0]); Serial.println(F(":"));
    debug_byte_array(buffer, 16); Serial.println();
        
    // Check that data in block is what we have written
    // by counting the number of bytes that are equal
    Serial.println(F("Checking result..."));
    byte count = 0;
    for (byte i = 0; i < 16; i++) {
        // Compare buffer (= what we've read) with DATA_RFID (= what we've written)
        if (buffer[i] == DATA_RFID[i])
            count++;
    }
    Serial.print(F("Number of bytes that match = ")); Serial.println(count);
    if (count == 16) {
        Serial.println(F("Success :-)"));
    } else {
        Serial.println(F("Failure, no match :-("));
        Serial.println(F("  perhaps the write didn't work properly..."));
    }
    Serial.println();
        
    // Dump the sector data
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector[0]);
    Serial.println();

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    
    debug_start();
    debug_data_http();
    debug_data_rfid();
    debug_end();    
} // End Loop

//-------------------------------------------------------------------------------//
//------------------------------- Start Clear Rfid ------------------------------//
//-------------------------------------------------------------------------------//
void clear_rfid (){      
    byte CLEAR_DATA [16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    byte k = 0;
    byte l = 0;
      
    for (byte i = 0; i < 15; i++){
        trailerBlock = trailer[l];
        status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK){
            flag_rfid = false;
        } else flag_rfid = true;
      
        for (byte j = 0; j < 3; j++){
            status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr[j+k], CLEAR_DATA, 16);
            if (status != MFRC522::STATUS_OK) {
                flag_rfid = false;
            } else flag_rfid = true;
        }
        k += 3;
        l++;
    }
       
    if (flag_rfid) {
        Serial.println(F("Clear Rfid Complete!"));
    } else Serial.println(F("Clear Rfid Error!"));        
}
//-------------------------------------------------------------------------------//
//------------------------------- End Clear Rfid --------------------------------//
//-------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------//
//---------------------------- Start Create Data Rfid ---------------------------//
//-------------------------------------------------------------------------------//
void create_data_rfid (byte *time){
    byte j = 0;
    for (byte i = 0; i < 2; i++) {
        DATA_RFID[i] = PK_ID[i];
    }
    for (byte i = 2; i < 8; i++) {
        j = i - 2;
        DATA_RFID[i] = time[j];
    }
    for (byte i = 8; i < 16; i++) {
        DATA_RFID[i] = 0;
    }
}

void create_data_http (byte *rfid_id, byte *time){
    byte j = 0;
    for (byte i = 0; i < 2; i++) {
        j = i;
        DATA_HTTP[i] = PK_ID[j];
    }
    for (byte i = 2; i < 6; i++) {
        j = i - 2;
        DATA_HTTP[i] = rfid_id[j];
    }
    for (byte i = 6; i < 12; i++) {
        j = i - 6;
        DATA_HTTP[i] = time[j];
    }
    for (byte i = 12; i < 16; i++) {
        DATA_HTTP[i] = 0;
    }
}

void debug_data_rfid (){
    Serial.print("Data Rfid:  ");
    for (byte i = 0; i < 2; i++) {
        Serial.print((char)DATA_RFID[i]);  // Print PK ID: A B
        Serial.print(' ');
    }
    for (byte i = 2; i < 16; i++) {
        Serial.print(DATA_RFID[i], HEX);  // Print Card ID: A B C D 
        Serial.print(' ');        
    }
    Serial.println();
}
//-------------------------------------------------------------------------------//
//----------------------------- End Debug Data Rfid -----------------------------//
//-------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------//
//---------------------------- Start Debug Data Http ----------------------------//
//-------------------------------------------------------------------------------//
void debug_data_http (){
    Serial.print("PK ID:      ");
    for (byte i = 0; i < 2; i++) {
        Serial.print((char)DATA_HTTP[i]);  // Print PK ID: A B
        Serial.print(' ');
    }
    Serial.println();
    
    Serial.print("Rfid ID:    ");
    for (byte i = 2; i < 6; i++) {
        Serial.print(DATA_HTTP[i], HEX);  // Print Card ID: A B C D 
        Serial.print(' ');        
    }
    Serial.println();
    
    Serial.print("Time:       ");
    for (byte i = 6; i < 9; i++) {
        Serial.print(DATA_HTTP[i], HEX);  // Print Date: YY MM DD
          if ((i == 6)|(i == 7)) Serial.print('/');
    }
    Serial.print(' ');
    for (byte i = 9; i < 12; i++) {
        Serial.print(DATA_HTTP[i], HEX);  // Print Time: HH MM SS
          if ((i == 9)|(i == 10)) Serial.print(':');
    }
    Serial.println();

    Serial.print("Data Http:  ");
    for (byte i = 0; i < 2; i++) {
        Serial.print((char)DATA_HTTP[i]);  // Print PK ID: A B
        Serial.print(' ');
    }
    
    for (byte i = 2; i < 16; i++) {
        Serial.print(DATA_HTTP[i], HEX);  // Print Card Rest 
        Serial.print(' ');        
    }
    Serial.println();
}
//-------------------------------------------------------------------------------//
//----------------------------- End Debug Data Http -----------------------------//
//-------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------//
//---------------------------- Start Debug Byte Array ---------------------------//
//-------------------------------------------------------------------------------//
void debug_byte_array(byte *buffer, byte bufferSize) { 
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
    Serial.println();
}
//-------------------------------------------------------------------------------//
//---------------------------- End Debug Byte Array -----------------------------//
//-------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------//
//------------------------------ Start Debug Start ------------------------------//
//-------------------------------------------------------------------------------//
void debug_start (){
    Serial.println();
    Serial.println("************************ Start Debug ************************");
    Serial.println();
}  
//-------------------------------------------------------------------------------//
//------------------------------- End Debug Start -------------------------------//
//-------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------//
//------------------------------- Start Debug End -------------------------------//
//-------------------------------------------------------------------------------//
void debug_end (){
    Serial.println();
    Serial.println("************************* End Debug *************************");
    Serial.println();
}
