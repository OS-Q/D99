#ifndef MifareUltralight_h
#define MifareUltralight_h

#include "./NFC.h"
#include "./NfcTag.h"
#include "./Ndef.h"

class MifareUltralight
{
    public:
        MifareUltralight(NFC& nfcShield);
        ~MifareUltralight();
        NfcTag read(byte *uid, unsigned int uidLength);
        boolean write(NdefMessage& ndefMessage, byte *uid, unsigned int uidLength);
        boolean clean();
    private:
        NFC* nfc;
        unsigned int tagCapacity;
        unsigned int messageLength;
        unsigned int bufferSize;
        unsigned int ndefStartIndex;
        boolean isUnformatted();
        void readCapabilityContainer();
        void findNdefMessage();
        void calculateBufferSize();
};

#endif
