#include "ChainPIR.hpp"
chain_status_t ChainPIR::getIRStatus(uint8_t id, pir_detect_result_t *ir_status, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_PIR_GET_IR_STATUS, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_PIR_GET_IR_STATUS, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                // 这里传参要返回的数据
                *ir_status = (pir_detect_result_t)returnPacket[6];
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


chain_status_t ChainPIR::setPIRDetectTriggerMode(uint8_t id, chain_detect_mode_t auto_status, uint8_t* operationStatus, unsigned long timeout)
{
     // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        sendPacket(id, CHAIN_PIR_SET_AUTO_SEND_IR_STATUS, (uint8_t*)&auto_status, 1);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_PIR_SET_AUTO_SEND_IR_STATUS, timeout)) {
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

chain_status_t ChainPIR::getPIRDetectTriggerMode(uint8_t id, chain_detect_mode_t* auto_status, unsigned long timeout)
{
     // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_PIR_GET_AUTO_SEND_IR_STATUS, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_PIR_GET_AUTO_SEND_IR_STATUS, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(returnPacket), returnPacketSize)) {
                // 这里传参要返回的数据
                *auto_status = (chain_detect_mode_t)returnPacket[6];
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
uint16_t ChainPIR::getPIRTypeCode(void)
{
    return CHAIN_PIR_DEVICE_TYPE_CODE;
}

bool ChainPIR::getPIRDetectTrigger(uint8_t id, pir_detect_report_t *triggerStatus)
{
    processIncomingData();
    bool findStatus = 0;
    record_info_t result;
    findStatus = findRecord(&recordList, id, &result);
    if (findStatus == true) {
        *triggerStatus = (pir_detect_report_t)result.type;
        return true;
    }
    return false;
}