#include "ChainAngle.h"

chain_status_t ChainAngle::getAngle12Adc(uint8_t id, uint16_t *adcValue, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ANGLE_GET_12ADC, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_ANGLE_GET_12ADC, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                // 这里传参要返回的数据
                *adcValue = (returnPacket[7] << 8) | returnPacket[6];
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
chain_status_t ChainAngle::getAngle8Adc(uint8_t id, uint8_t *adcValue, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ANGLE_PGET_8ADC, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_ANGLE_PGET_8ADC, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                // 这里传参要返回的数据
                *adcValue = returnPacket[6];
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
chain_status_t ChainAngle::setAngleClockwise(uint8_t id, uint8_t direction, uint8_t *operationStatus,
                                             uint8_t saveToFlash, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = direction;
        cmdBuffer[cmdBufferSize++] = saveToFlash;
        sendPacket(id, CHAIN_ANGLE_SET_CLOCKWISE_STATUS, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_ANGLE_SET_CLOCKWISE_STATUS, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                // 这里传参要返回的数据
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
chain_status_t ChainAngle::getAngleClockwise(uint8_t id, uint8_t *direction, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_ANGLE_GET_CLOCKWISE_STATUS, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_ANGLE_GET_CLOCKWISE_STATUS, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                // 这里传参要返回的数据
                *direction = returnPacket[6];
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

uint16_t ChainAngle::getAngleTypeCode(void)
{
    return CHAIN_ANGLE_DEVICE_TYPE_CODE;
}