#include "ChainJoystick.h"

chain_status_t ChainJoystick::getJoystick16Adc(uint8_t id, uint16_t *xAdcValue, uint16_t *yAdcValue,
                                               unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_JOYSTICK_GET_16ADC, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_JOYSTICK_GET_16ADC, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *xAdcValue = (cmdReturnBuffer[7] << 8) | cmdReturnBuffer[6];
                *yAdcValue = (cmdReturnBuffer[9] << 8) | cmdReturnBuffer[8];
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
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_JOYSTICK_GET_8ADC, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_JOYSTICK_GET_8ADC, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *xAdcValue = cmdReturnBuffer[6];
                *yAdcValue = cmdReturnBuffer[7];
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
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_RANGE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_RANGE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                for (uint8_t i = 0; i < 8; i++) {
                    mapBufValue[i] = (cmdReturnBuffer[7 + i * 2] << 8) | cmdReturnBuffer[6 + i * 2];
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
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        for (uint8_t i = 0; i < 8; i++) {
            cmdBuffer[cmdBufferSize++] = mapBufValue[i] & 0xFF;
            cmdBuffer[cmdBufferSize++] = (mapBufValue[i] >> 8) & 0xFF;
        }
        cmdBuffer[cmdBufferSize++] = saveToFlash;
        sendPacket(id, CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_JOYSTICK_SET_ADC_XY_MAPPED_RANGE, timeout)) {
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
chain_status_t ChainJoystick::getJoystickMappedInt16Value(uint8_t id, int16_t *xMapAdcValue, int16_t *yMapAdcValue,
                                                          unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT16_VALUE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT16_VALUE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *xMapAdcValue = (cmdReturnBuffer[7] << 8) | cmdReturnBuffer[6];
                *yMapAdcValue = (cmdReturnBuffer[9] << 8) | cmdReturnBuffer[8];
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
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT8_VALUE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT8_VALUE, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *xMapAdcValue = cmdReturnBuffer[6];
                *yMapAdcValue = cmdReturnBuffer[7];
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
    return CHAIN_JOYSTICK_TYPE_CODE;
}