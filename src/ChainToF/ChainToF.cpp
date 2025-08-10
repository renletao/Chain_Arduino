/*
 *SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#include "ChainToF.hpp"

chain_status_t ChainToF::getToFDistance(uint8_t id, uint16_t *distance, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;

    if (acquireMutex()) {
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_TOF_GET_DISTANCE, cmdBuffer, cmdBufferSize);
        if (waitForData(id, CHAIN_TOF_GET_DISTANCE, timeout)) {
            if (checkPacket(returnPacket, returnPacketSize)) {
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

chain_status_t ChainToF::setToFMeasurementTime(uint8_t id, uint8_t time, uint8_t *operationStatus,
                                    unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;

    if (time < TOF_MEASUREMENT_TIME_MIN || time > TOF_MEASUREMENT_TIME_MAX) {
        return CHAIN_PARAMETER_ERROR;
    }

    if (acquireMutex()) {
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = time;
        sendPacket(id, CHAIN_TOF_SET_MEASUREMENT_TIME, cmdBuffer, cmdBufferSize);
        if (waitForData(id, CHAIN_TOF_SET_MEASUREMENT_TIME, timeout)) {
            if (checkPacket(returnPacket, returnPacketSize)) {
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

chain_status_t ChainToF::getToFMeasurementTime(uint8_t id, uint8_t *time, unsigned long timeout)
{
    chain_status_t status = CHAIN_OK;

    if (acquireMutex()) {
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_TOF_GET_MEASUREMENT_TIME, cmdBuffer, cmdBufferSize);

        if (waitForData(id, CHAIN_TOF_GET_MEASUREMENT_TIME, timeout)) {
            if (checkPacket(returnPacket, returnPacketSize)) {
                *time = returnPacket[6];
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
