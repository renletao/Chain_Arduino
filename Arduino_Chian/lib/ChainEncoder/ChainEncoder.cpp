#include "ChainEncoder.h"

chain_status_t ChainEncoder::getEncoderValue(uint8_t id, int16_t *value, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ENCODER_GET_VALUE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_ENCODER_GET_VALUE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *value = (int16_t)((cmdReturnBuffer[7] << 8) | cmdReturnBuffer[6]);
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
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ENCODER_GET_INC_VALUE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_ENCODER_GET_INC_VALUE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *incValue = (int16_t)((cmdReturnBuffer[7] << 8) | cmdReturnBuffer[6]);
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
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ENCODER_RESET_VALUE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_ENCODER_RESET_VALUE, timeout)) {
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
chain_status_t ChainEncoder::resetEncoderIncValue(uint8_t id, uint8_t *operationStatus, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ENCODER_RESET_INC_VALUE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_ENCODER_RESET_INC_VALUE, timeout)) {
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
chain_status_t ChainEncoder::setEncoderABDirect(uint8_t id, uint8_t direct, uint8_t *operationStatus,
                                                uint8_t saveToFlash, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = direct;
        cmdBuffer[cmdBufferSize++] = saveToFlash;
        sendPacket(id, CHAIN_ENCODER_SET_AB_STATUS, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_ENCODER_SET_AB_STATUS, timeout)) {
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
chain_status_t ChainEncoder::getEncoderABDirect(uint8_t id, uint8_t *direct, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ENCODER_GET_AB_STATUS, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_ENCODER_GET_AB_STATUS, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *direct = cmdReturnBuffer[6];
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
    return CHAIN_ENCODER_TYPE_CODE;
}