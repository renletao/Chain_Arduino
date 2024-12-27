#include "ChainJoystick.h"

chain_status_t ChainJoystick::getJoystick16Adc(uint8_t id, uint16_t *xAdcValue, uint16_t *yAdcValue,
                                               unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_JOYSTICK_GET_16ADC, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_JOYSTICK_GET_16ADC, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *xAdcValue = (returnPacket[7] << 8) | returnPacket[6];
                *yAdcValue = (returnPacket[9] << 8) | returnPacket[8];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}
chain_status_t ChainJoystick::getJoystick8Adc(uint8_t id, uint8_t *xAdcValue, uint8_t *yAdcValue, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_JOYSTICK_GET_8ADC, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_JOYSTICK_GET_8ADC, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *xAdcValue = returnPacket[6];
                *yAdcValue = returnPacket[7];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}
chain_status_t ChainJoystick::getJoystickMappedRange(uint8_t id, uint16_t *mapBufValue, uint8_t size,
                                                     unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_RANGE, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_RANGE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                for (uint8_t i = 0; i < 8; i++) {
                    mapBufValue[i] = (returnPacket[7 + i * 2] << 8) | returnPacket[6 + i * 2];
                }
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}
chain_status_t ChainJoystick::setJoystickMappedRange(uint8_t id, uint16_t *mapBufValue, uint8_t size,
                                                     uint8_t *operationStatus, uint8_t saveToFlash,
                                                     unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        for (uint8_t i = 0; i < 8; i++) {
            cmdBuffer[cmdBufferSize++] = mapBufValue[i] & 0xFF;
            cmdBuffer[cmdBufferSize++] = (mapBufValue[i] >> 8) & 0xFF;
        }
        cmdBuffer[cmdBufferSize++] = saveToFlash;
        sendPacket(id, CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *operationStatus = returnPacket[6];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}
chain_status_t ChainJoystick::getJoystickMappedInt16Value(uint8_t id, int16_t *xMapAdcValue, int16_t *yMapAdcValue,
                                                          unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT16_VALUE, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT16_VALUE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *xMapAdcValue = (returnPacket[7] << 8) | returnPacket[6];
                *yMapAdcValue = (returnPacket[9] << 8) | returnPacket[8];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}
chain_status_t ChainJoystick::getJoystickMappedInt8Value(uint8_t id, int8_t *xMapAdcValue, int8_t *yMapAdcValue,
                                                         unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT8_VALUE, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT8_VALUE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *xMapAdcValue = returnPacket[6];
                *yMapAdcValue = returnPacket[7];
            } else {
                status = CHAIN_RETURN_PACKET_ERROR;
            }
        } else {
            status = CHAIN_TIMEOUT;
        }
        releaseMutex();
    } else {
        status = CHAIN_BUSY;
    }
    return status;
}

uint16_t ChainJoystick::getJoystickTypeCode(void)
{
    return CHAIN_JOYSTICK_DEVICE_TYPE_CODE;
}

bool ChainJoystick::getJoystickButtonPressStatus(uint8_t id)
{
    bool findStatus = 0;
    record_info_t result;
    processIncomingData();
    findStatus = findRecord(&recordList, id, &result);
    if (findStatus == true && result.type == CHAIN_JOYSTICK_BUTTON_PRESS_TYPE_CODE) {
        return true;
    }
    return false;
}