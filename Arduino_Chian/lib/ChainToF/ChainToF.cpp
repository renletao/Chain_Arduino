#include "ChainToF.h"

chain_status_t ChainToF::getTofDistance(uint8_t id, uint16_t *distance, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_TOF_GET_DISTANCE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_TOF_GET_DISTANCE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *distance = (cmdReturnBuffer[7] << 8) | cmdReturnBuffer[6];
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
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = mode;
        cmdBuffer[cmdBufferSize++] = saveToFlash;
        sendPacket(id, CHAIN_TOF_SET_MODE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_TOF_SET_MODE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *operationStatus = cmdReturnBuffer[6];
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
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_TOF_GET_MODE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_TOF_GET_MODE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *mode = cmdReturnBuffer[6];
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
    return CHAIN_TOF_TYPE_CODE;
}