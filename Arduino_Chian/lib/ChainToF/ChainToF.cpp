#include "ChainToF.h"

chain_status_t ChainToF::getTofDistance(uint8_t id, uint16_t *distance, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_TOF_GET_DISTANCE, cmdBuffer, cmdBufferSize);
        if (waitForData(id, CHAIN_TOF_GET_DISTANCE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *distance = (returnPacket[7] << 8) | returnPacket[6];
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
chain_status_t ChainToF::setTofMode(uint8_t id, uint8_t mode, uint8_t *operationStatus, uint8_t saveToFlash,
                                    unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (!(mode >= TOF_SHORT && mode <= TOF_LONG)) {
        status = CHAIN_PARAMETER_ERROR;
        return status;
    }
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = mode;
        cmdBuffer[cmdBufferSize++] = saveToFlash;
        sendPacket(id, CHAIN_TOF_SET_MODE, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_TOF_SET_MODE, timeout)) {
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
chain_status_t ChainToF::getTofMode(uint8_t id, uint8_t *mode, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_TOF_GET_MODE, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_TOF_GET_MODE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                *mode = returnPacket[6];
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

uint16_t ChainToF::getToFTypeCode(void)
{
    return CHAIN_TOF_DEVICE_TYPE_CODE;
}