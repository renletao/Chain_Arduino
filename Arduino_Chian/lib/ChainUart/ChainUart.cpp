#include "ChainUart.h"
chain_status_t ChainUart::chianUartI2cInit(uint8_t id, uint8_t i2cSpeed, uint8_t *operationStatus,
                                           unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = i2cSpeed;
        sendPacket(id, CHAIN_I2C_INIT, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_I2C_INIT, timeout)) {
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
chain_status_t ChainUart::chainUartI2cRead(uint8_t id, uint8_t i2cAddr, uint8_t readLength, uint8_t *buffer,
                                           uint8_t *operationStatus, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (readLength > I2C_READ_MAX_SIZE || i2cAddr > I2C_ADDR_MAX) {
        status = CHAIN_PARAMETER_ERROR;
        return status;
    }
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = i2cAddr;
        cmdBuffer[cmdBufferSize++] = readLength;
        sendPacket(id, CHAIN_I2C_READ, cmdBuffer, cmdBufferSize);
        // 这里等待接收数据
        if (waitForData(id, CHAIN_I2C_READ, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *operationStatus = cmdReturnBuffer[6];
                for (uint8_t i = 0; i < readLength; i++) {
                    buffer[i] = cmdReturnBuffer[7 + i];
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
chain_status_t ChainUart::chainUartI2cWrite(uint8_t id, uint8_t i2cAddr, uint8_t writeLength, uint8_t *buffer,
                                            uint8_t *operationStatus, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (writeLength > I2C_READ_MAX_SIZE || i2cAddr > I2C_WRITE_MAX_SIZE) {
        status = CHAIN_PARAMETER_ERROR;
        return status;
    }
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = i2cAddr;
        cmdBuffer[cmdBufferSize++] = writeLength;
        for (uint8_t i = 0; i < writeLength; i++) {
            cmdBuffer[cmdBufferSize++] = buffer[i];
        }
        sendPacket(id, CHAIN_I2C_WRITE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_I2C_WRITE, timeout)) {
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
chain_status_t ChainUart::chainUartI2cMemRead(uint8_t id, uint8_t i2cAddr, uint16_t regAddr, uint8_t regLength,
                                              uint8_t readLength, uint8_t *buffer, uint8_t *operationStatus,
                                              unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (i2cAddr > I2C_ADDR_MAX || readLength > I2C_READ_MAX_SIZE) {
        status = CHAIN_PARAMETER_ERROR;
        return status;
    }
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = i2cAddr;
        cmdBuffer[cmdBufferSize++] = regLength;
        cmdBuffer[cmdBufferSize++] = regAddr & 0xFF;
        cmdBuffer[cmdBufferSize++] = (regAddr >> 8) & 0xFF;
        cmdBuffer[cmdBufferSize++] = readLength;

        sendPacket(id, CHAIN_I2C_MEM_READ, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_I2C_MEM_READ, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *operationStatus = cmdReturnBuffer[6];
                for (uint8_t i = 0; i < readLength; i++) {
                    buffer[i] = cmdReturnBuffer[7 + i];
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
chain_status_t ChainUart::chainUartI2cMemWrite(uint8_t id, uint8_t i2cAddr, uint16_t regAddr, uint8_t regLength,
                                               uint8_t writeLength, uint8_t *buffer, uint8_t *operationStatus,
                                               unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (i2cAddr > I2C_ADDR_MAX || writeLength > I2C_WRITE_MAX_SIZE) {
        status = CHAIN_PARAMETER_ERROR;
        return status;
    }
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = i2cAddr;
        cmdBuffer[cmdBufferSize++] = regLength;
        cmdBuffer[cmdBufferSize++] = regAddr & 0xFF;
        cmdBuffer[cmdBufferSize++] = (regAddr >> 8) & 0xFF;
        cmdBuffer[cmdBufferSize++] = writeLength;
        for (uint8_t i = 0; i < writeLength; i++) {
            cmdBuffer[cmdBufferSize++] = buffer[i];
        }

        sendPacket(id, CHAIN_I2C_MEM_WRITE, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_I2C_MEM_WRITE, timeout)) {
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
chain_status_t ChainUart::getChainUartI2cDeviceAddr(uint8_t id, uint8_t *i2cAddrNums, uint8_t *buffer,
                                                    uint8_t *operationStatus, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_I2C_GET_DEVICE_ADDR, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_I2C_GET_DEVICE_ADDR, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *operationStatus = cmdReturnBuffer[6];
                *i2cAddrNums     = cmdReturnBuffer[7];
                for (uint8_t i = 0; i < cmdReturnBuffer[7]; i++) {
                    buffer[i] = cmdReturnBuffer[8 + i];
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
chain_status_t ChainUart::chainUartOutputInit(uint8_t id, uint8_t gpio, uint8_t gpioLevel, uint8_t gpioOutput,
                                              uint8_t gpioPull, uint8_t gpioSpeed, uint8_t *operationStatus,
                                              unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = gpio;
        cmdBuffer[cmdBufferSize++] = gpioLevel;
        cmdBuffer[cmdBufferSize++] = gpioOutput;
        cmdBuffer[cmdBufferSize++] = gpioPull;
        cmdBuffer[cmdBufferSize++] = gpioSpeed;
        sendPacket(id, CHAIN_GPIO_OUTPUT_INIT, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_GPIO_OUTPUT_INIT, timeout)) {
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
chain_status_t ChainUart::chainUartInputInit(uint8_t id, uint8_t gpio, uint8_t gpioPull, uint8_t *operationStatus,
                                             unsigned long timeout)
{  // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = gpio;
        cmdBuffer[cmdBufferSize++] = gpioPull;
        sendPacket(id, CHAIN_GPIO_INPUT_INIT, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_GPIO_INPUT_INIT, timeout)) {
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
chain_status_t ChainUart::chainUartReadLevel(uint8_t id, uint8_t gpio, uint8_t *gpioLevel, uint8_t *operationStatus,
                                             unsigned long timeout)
{  // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = gpio;
        sendPacket(id, CHAIN_GPIO_READ_GPIO_LEVEL, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_GPIO_READ_GPIO_LEVEL, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *operationStatus = cmdReturnBuffer[6];
                *gpioLevel       = cmdReturnBuffer[7];
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
chain_status_t ChainUart::chainUartNvicInit(uint8_t id, uint8_t gpio, uint8_t gpioPull, uint8_t triggerMode,
                                            uint8_t *operationStatus, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = gpio;
        cmdBuffer[cmdBufferSize++] = gpioPull;
        cmdBuffer[cmdBufferSize++] = triggerMode;
        sendPacket(id, CHAIN_GET_BOOTLOADER_VERSION, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_GET_BOOTLOADER_VERSION, timeout)) {
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
chain_status_t ChainUart::chainUartAdcInit(uint8_t id, uint8_t adcChannel1, uint8_t adcChannel2,
                                           uint8_t *operationStatus, unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize              = 0;
        cmdBuffer[cmdBufferSize++] = adcChannel1;
        cmdBuffer[cmdBufferSize++] = adcChannel2;
        sendPacket(id, CHAIN_GET_BOOTLOADER_VERSION, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_GET_BOOTLOADER_VERSION, timeout)) {
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
chain_status_t ChainUart::getAdcValue(uint8_t id, uint8_t *adcChannelNums, uint8_t *buffer, uint8_t *operationStatus,
                                      unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_GET_BOOTLOADER_VERSION, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_GET_BOOTLOADER_VERSION, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *operationStatus = cmdReturnBuffer[6];
                *adcChannelNums  = cmdReturnBuffer[7];
                for (uint8_t i = 0; i < cmdReturnBuffer[7] * 2; i++) {
                    buffer[i] = cmdReturnBuffer[8 + i];
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
chain_status_t ChainUart::getUartStatus(uint8_t id, uint8_t *gpio11WorkStatus, uint8_t *gpio12WorkStatus,
                                        unsigned long timeout)
{
    // 指令处理模板
    chain_status_t status = CHAIN_OK;
    if (acquireMutex()) {
        // 这里发送数据
        memset(cmdBuffer, 0, cmdBufferSize);
        cmdBufferSize = 0;
        sendPacket(id, CHAIN_GET_BOOTLOADER_VERSION, cmdBuffer, cmdBufferSize);

        // 这里等待接收数据
        if (waitForData(id, CHAIN_GET_BOOTLOADER_VERSION, timeout)) {
            if (checkPacket(reinterpret_cast<const uint8_t *>(cmdReturnBuffer), cmdReturnBufferSize)) {
                // 这里传参要返回的数据
                *gpio11WorkStatus = cmdReturnBuffer[6];
                *gpio12WorkStatus = cmdReturnBuffer[7];
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

uint16_t ChainUart::getUartTypeCode(void)
{
    return CHAIN_UART_TYPE_CODE;
}