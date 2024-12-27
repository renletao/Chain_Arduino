#include "ChainKey.h"

uint16_t ChainKey::getKeyTypeCode(void)
{
    return CHAIN_KEY_DEVICE_TYPE_CODE;
}

bool ChainKey::getKeyButtonPressStatus(uint8_t id)
{
    processIncomingData();
    bool findStatus = 0;
    record_info_t result;
    findStatus = findRecord(&recordList, id, &result);
    if (findStatus == true && result.type == CHAIN_KEY_BUTTON_PRESS_TYPE_CODE) {
        return true;
    }
    return false;
}