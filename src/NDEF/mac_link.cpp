
#include "./mac_link.h"
#include "./PN532_debug.h"

int8_t MACLink::activateAsTarget(uint16_t timeout)
{
	NFC.begin();
	NFC.SAMConfig();
    return NFC.tgInitAsTarget(timeout);
}

bool MACLink::write(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
    return NFC.tgSetData(header, hlen, body, blen);
}

int16_t MACLink::read(uint8_t *buf, uint8_t len)
{
    return NFC.tgGetData(buf, len);
}
