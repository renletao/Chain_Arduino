#include "ChainEncoder.h"

chain_status_t ChainEncoder::getEncoderValue(uint8_t id, int16_t *value, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ENCODER_GET_VALUE, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_ENCODER_GET_VALUE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *value = (int16_t)((returnPacket[7] << 8) | returnPacket[6]);
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
chain_status_t ChainEncoder::getEncoderIncValue(uint8_t id, int16_t *incValue, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ENCODER_GET_INC_VALUE, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_ENCODER_GET_INC_VALUE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *incValue = (int16_t)((returnPacket[7] << 8) | returnPacket[6]);
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
chain_status_t ChainEncoder::resetEncoderValue(uint8_t id, uint8_t *operationStatus, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ENCODER_RESET_VALUE, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_ENCODER_RESET_VALUE, timeout)) {
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
chain_status_t ChainEncoder::resetEncoderIncValue(uint8_t id, uint8_t *operationStatus, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ENCODER_RESET_INC_VALUE, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_ENCODER_RESET_INC_VALUE, timeout)) {
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
chain_status_t ChainEncoder::setEncoderABDirect(uint8_t id, uint8_t direct, uint8_t *operationStatus,
                                                uint8_t saveToFlash, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (!(direct == ENCODER_AB || direct == ENCODER_BA)) {
        status = CHAIN_PARAMETER_ERROR;
        return status;
    }
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = direct;
        cmdBuffer[cmdBufferSize++] = saveToFlash;
        sendPacket(id, CHAIN_ENCODER_SET_AB_STATUS, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_ENCODER_SET_AB_STATUS, timeout)) {
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
chain_status_t ChainEncoder::getEncoderABDirect(uint8_t id, uint8_t *direct, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ENCODER_GET_AB_STATUS, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_ENCODER_GET_AB_STATUS, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *direct = returnPacket[6];
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
uint16_t ChainEncoder::getEncoderTypeCode(void)
{
    return CHAIN_ENCODER_DEVICE_TYPE_CODE;
}
bool ChainEncoder::getEncoderButtonPressStatus(uint8_t id)
{
    processIncomingData();
    bool findStatus = 0;
    record_info_t result;
    findStatus = findRecord(&recordList, id, &result);
    if (findStatus == true && result.type == CHAIN_ENCODER_BUTTON_PRESS_TYPE_CODE) {
        return true;
    }
    return false;
}