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
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         5           // Configurable, see typical pin layout above
#define SS_PIN          15           // Configurable, see typical pin layout above
#define IRQ_PIN         4          

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.



volatile boolean bNewInt = false;
unsigned char regVal = 0x7F;
void activateRec(MFRC522 mfrc522);
void clearInt(MFRC522 mfrc522);

byte nuidPICC[4];

MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;







    byte sector[15]    = { 1,  2,  3,    4,  5,  6,    7,  8,  9,   10, 11, 12,   13, 14, 15 };
    byte blockAddr[45] = { 4,  5,  6,    8,  9, 10,   12, 13, 14,   16, 17, 18,   20, 21, 22,   
                          24, 25, 26,   28, 29, 30,   32, 33, 34,   36, 37, 38,   40, 41, 42,   
                          44, 45, 46,   48, 49, 50,   52, 53, 54,   56, 57, 58,   60, 61, 62 };
        
    byte trailer[15]  = { 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63 };
    byte trailerBlock = trailer[0];
    
    boolean flag_rfid = true;
    
    
    byte buffer[18];
    byte size = sizeof(buffer);

/**
 * MFRC522 interrupt serving routine
 */
void ICACHE_RAM_ATTR readCard(){
   bNewInt = true;
}


/**
 * Initialize.
 */
void setup() {
    Serial.begin(115200); // Initialize serial communications with the PC
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();        // Init SPI bus

    debug_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    mfrc522.PCD_Init(); // Init MFRC522 card
    
    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
    }

    /* read and printout the MFRC522 version (valid values 0x91 & 0x92)*/
    Serial.print("Ver: 0x");      
    byte readReg = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
    Serial.println(readReg, HEX);      

    /* setup the IRQ pin*/
    pinMode(IRQ_PIN, INPUT_PULLUP);
   
    /* 
     *  Allow the ... irq to be propagated to the IRQ pin
     *  For test purposes propagate the IdleIrq and loAlert
     */
    regVal = 0xA0; //rx irq
    mfrc522.PCD_WriteRegister(mfrc522.ComIEnReg,regVal);
    
    bNewInt = false; //interrupt flag

    /*Activate the interrupt*/
    attachInterrupt(IRQ_PIN, readCard, FALLING);
    
/*    do{ //clear a spourious interrupt at start
      ;
    }while(!bNewInt);
*/ 
    Serial.println("End setup"); 
}

/**
 * Main loop.
 */
void loop() {      
  
   if(bNewInt){  //new read interrupt
//       bNewInt = false;
//       detachInterrupt(IRQ_PIN);
       Serial.print("Interrupt. ");          
               
       // Look for new cards
       if ( ! mfrc522.PICC_IsNewCardPresent())
       return;

       // Verify if the NUID has been readed
       if ( ! mfrc522.PICC_ReadCardSerial())
       return;

       if (mfrc522.uid.uidByte[0] != nuidPICC[0] || 
          mfrc522.uid.uidByte[1] != nuidPICC[1] || 
          mfrc522.uid.uidByte[2] != nuidPICC[2] || 
          mfrc522.uid.uidByte[3] != nuidPICC[3] ) {
          Serial.println(F("A new card has been detected."));

          // Store NUID into nuidPICC array
          for (byte i = 0; i < 4; i++) {
              nuidPICC[i] = mfrc522.uid.uidByte[i];
          }
          
          Serial.println(F("ID:"));
          Serial.print(F("In hex: "));
          dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
          Serial.println();
       }else {
          Serial.println(F("Card read previously."));
          Serial.print(F("In hex: "));
          dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
          Serial.println();
       } 







/*




    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK){
          flag_rfid = false;
          return;
      }else flag_rfid = true;
      
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr[0], buffer, &size);
      if (status != MFRC522::STATUS_OK) {
          flag_rfid = false;
          return;
      }else flag_rfid = true;  
  



    debug_byte_array(buffer, 16); Serial.println();
    Serial.println();










*/



   
      
/*      mfrc522.PICC_ReadCardSerial(); //read the tag data
      // Show some details of the PICC (that is: the tag/card)
      //Serial.print(F("Card UID:"));
      dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
      Serial.println();
*/    

      mfrc522.PICC_HaltA();
//      mfrc522.PCD_StopCrypto1();

      clearInt(mfrc522);
      bNewInt = false;
      /*Activate the interrupt*/
//      attachInterrupt(IRQ_PIN, readCard, FALLING);
   }

// The receiving block needs regular retriggering (tell the tag it should transmit??)
// (mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg,mfrc522.PICC_CMD_REQA);)
activateRec(mfrc522);
//   delay(100);
   /*Activate the interrupt*/
} //loop()

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}


/*
 * The function sending to the MFRC522 the needed commands to activate the reception
 */
void activateRec(MFRC522 rfid){
    mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg,mfrc522.PICC_CMD_REQA);
    mfrc522.PCD_WriteRegister(mfrc522.CommandReg,mfrc522.PCD_Transceive);  
    mfrc522.PCD_WriteRegister(mfrc522.BitFramingReg, 0x87);    
}

/*
 * The function to clear the pending interrupt bits after interrupt serving routine
 */
void clearInt(MFRC522 mfrc522){
   mfrc522.PCD_WriteRegister(mfrc522.ComIrqReg,0x7F);
}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}




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
