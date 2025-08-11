#include "ChainSwitch.hpp"

chain_status_t ChainSwitch::getSwitch12BitAdc(uint8_t id, uint16_t *adcValue, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_SWITCH_GET_12ADC, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_SWITCH_GET_12ADC, timeout)) {
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
chain_status_t ChainSwitch::getSwitch8BitAdc(uint8_t id, uint8_t *adcValue, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_SWITCH_PGET_8ADC, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_SWITCH_PGET_8ADC, timeout)) {
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
chain_status_t ChainSwitch::setSwitchDirection(uint8_t id, switch_direction_t direction, uint8_t *operationStatus,
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
        sendPacket(id, CHAIN_SWITCH_SET_DOWNUP_STATUS, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_SWITCH_SET_DOWNUP_STATUS, timeout)) {
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
chain_status_t ChainSwitch::getSwitchDirection(uint8_t id, switch_direction_t *direction, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_SWITCH_GET_DOWNUP_STATUS, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_SWITCH_GET_DOWNUP_STATUS, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                // 这里传参要返回的数据
                *direction = (switch_direction_t)returnPacket[6];
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

chain_status_t ChainSwitch::setSwitchThresholdValue(uint8_t id, uint16_t open_threshold, uint16_t close_threshold, uint8_t *operationStatus, 
                                    uint8_t saveToFlash, unsigned long timeout)
{
     // 指令处理模板
    chain_status_t status = CHAIN_OK;

    if ((open_threshold > 4095) || (close_threshold > 4095) || (open_threshold < close_threshold)) {
        return CHAIN_PARAMETER_ERROR; // 参数错误
    }

    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = open_threshold & 0xFF;
        cmdBuffer[cmdBufferSize++] = (open_threshold >> 8) & 0xFF;
        cmdBuffer[cmdBufferSize++] = close_threshold & 0xFF;
        cmdBuffer[cmdBufferSize++] = (close_threshold >> 8) & 0xFF;
        cmdBuffer[cmdBufferSize++] = saveToFlash;
        sendPacket(id, CHAIN_SWITCH_SET_THRESHOLD, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_SWITCH_SET_THRESHOLD, timeout)) {
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

chain_status_t ChainSwitch::getSwitchThresholdValue(uint8_t id, uint16_t* open_threshold, uint16_t* close_threshold, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_SWITCH_GET_THRESHOLD, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_SWITCH_GET_THRESHOLD, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                // 这里传参要返回的数据
                *open_threshold = returnPacket[6] | (returnPacket[7] << 8);
                *close_threshold = returnPacket[8] | (returnPacket[9] << 8);
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

chain_status_t ChainSwitch::getSwitchStatus(uint8_t id, uint8_t *switch_status, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_SWITCH_GET_SWITCH_STATUS, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_SWITCH_GET_SWITCH_STATUS, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                // 这里传参要返回的数据
                *switch_status = returnPacket[6];
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

chain_status_t ChainSwitch::setSwitchAutoTriggerMode(uint8_t id, chain_slip_mode_t auto_status, uint8_t* operationStatus, unsigned long timeout)
{
     // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = auto_status;
        sendPacket(id, CHAIN_SWITCH_SET_AUTO_SEND_SWITCH_STATUS, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_SWITCH_SET_AUTO_SEND_SWITCH_STATUS, timeout)) {
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

chain_status_t ChainSwitch::getSwitchAutoTriggerMode(uint8_t id, chain_slip_mode_t* auto_status, unsigned long timeout)
{
     // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_SWITCH_GET_AUTO_SEND_SWITCH_STATUS, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_SWITCH_GET_AUTO_SEND_SWITCH_STATUS, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                // 这里传参要返回的数据
                *auto_status = (chain_slip_mode_t)returnPacket[6];
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

uint16_t ChainSwitch::getSwitchTypeCode(void)
{
    return CHAIN_SWITCH_DEVICE_TYPE_CODE;
}

bool ChainSwitch::getSwitchTriggerStatus(uint8_t id, switch_trigger_type_t *triggerStatus)
{
    processIncomingData();
    bool findStatus = 0;
    record_info_t result;
    findStatus = findRecord(&recordList, id, &result);
    if (findStatus == true) {
        *triggerStatus = (switch_trigger_type_t)result.type;
        return true;
    }
    return false;
}
