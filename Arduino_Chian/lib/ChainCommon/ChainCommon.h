/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_COMMON_H
#define CHAIN_COMMON_H

#include <Arduino.h>

/**
 * @brief Timeout duration in milliseconds.
 */
#define TIMEOUT_MS (1)

/**
 * @brief Buffer size for receiving data.
 */
#define RECEIVE_BUFFER_SIZE (1024)

/**
 * @brief Buffer size for sending data.
 */
#define SEND_BUFFER_SIZE (256)

/**
 * @brief Buffer size for command storage.
 */
#define CMD_BUFFER_SIZE (256)

/**
 * @brief Buffer size for storing key states.
 */
#define KEY_BUFFER_SIZE (16)

/**
 * @brief High byte of the data packet header.
 */
#define PACK_HEAD_HIGH (0xAA)

/**
 * @brief Low byte of the data packet header.
 */
#define PACK_HEAD_LOW (0x55)

/**
 * @brief High byte of the data packet footer.
 */
#define PACK_END_HIGH (0x55)

/**
 * @brief Low byte of the data packet footer.
 */
#define PACK_END_LOW (0xAA)

/**
 * @brief Macro to indicate not saving to flash memory.
 */
#define SAVE_TO_FLASH_DISABLE (0)
/**
 * @brief Macro to indicate saving to flash memory.
 */
#define SAVE_TO_FLASH_ENABLE (1)

/**
 * @brief Maximum brightness level for RGB light.
 *
 * This macro defines the maximum brightness level for the RGB light, which is 100.
 */
#define RGB_MAX_BRIGHTNESS (100)

/**
 * @brief Status codes for chain command operations.
 */
typedef enum {
    CHAIN_OK                  = 0x00, /**< Operation successful. */
    CHAIN_PARAMETER_ERROR     = 0x01, /**< Parameter error. */
    CHAIN_RETURN_PACKET_ERROR = 0x02, /**< Return packet error. */
    CHAIN_BUSY                = 0x04, /**< Device is busy. */
    CHAIN_TIMEOUT             = 0x05  /**< Operation timeout. */
} chain_status_t;

/**
 * @brief RGB control commands.
 */
typedef enum {
    CHAIN_SET_RGB_VALUE = 0x20, /**< Set RGB value. */
    CHAIN_GET_RGB_VALUE = 0x21, /**< Get RGB value. */
    CHAIN_SET_RGB_LIGHT = 0x22, /**< Set RGB brightness. */
    CHAIN_GET_RGB_LIGHT = 0x23  /**< Get RGB brightness. */
} chain_rgb_cmd_t;

/**
 * @brief Basic commands for the chain series.
 */
typedef enum {
    CHAIN_GET_BOOTLOADER_VERSION = 0xF9, /**< Get Bootloader version. */
    CHAIN_GET_VERSION_DEVICE     = 0xFA, /**< Get device software version. */
    CHAIN_GET_DEVICE_TYPE        = 0xFB, /**< Get device type. */
    CHAIN_ENUM_PLEASE            = 0xFC, /**< Enumeration request. */
    CHAIN_HEARTBEAT              = 0xFD, /**< Heartbeat packet. */
    CHAIN_ENUM                   = 0xFE  /**< Enumeration response. */
} chain_base_cmd_t;

/**
 * @brief Device information structure.
 */
typedef struct {
    uint16_t id;          /**< Device ID. */
    uint16_t device_type; /**< Device type. */
} device_t;

/**
 * @brief List of devices structure.
 */
typedef struct {
    uint16_t count;    /**< Number of devices. */
    device_t *devices; /**< Array of devices. */
} device_list_t;

/**
 * @brief RGB color structure.
 */
typedef struct {
    uint8_t R; /**< Red component of the color. */
    uint8_t G; /**< Green component of the color. */
    uint8_t B; /**< Blue component of the color. */
} rgb_color;

class ChainCommon {
public:
    // Command buffer
    uint8_t cmdBuffer[CMD_BUFFER_SIZE] = {0};  // Buffer used to store commands that are to be sent.
    uint16_t cmdBufferSize             = 0;    // Current size of the command buffer (number of commands stored).

    // Returned command buffer
    uint8_t cmdReturnBuffer[CMD_BUFFER_SIZE] = {0};  // Buffer used to store the returned command data after execution.
    uint16_t cmdReturnBufferSize = 0;  // Current size of the returned command buffer (number of returned data bytes).

    /**
     * @brief Acquires the mutex to ensure exclusive access.
     *
     * This function attempts to acquire the mutex lock. If the mutex is already locked,
     * the function will return false. If successful, the mutex is locked and access is granted.
     *
     * @return true if the mutex was successfully acquired, false otherwise
     */
    bool acquireMutex(void);

    /**
     * @brief Releases the mutex to allow access by other processes.
     *
     * This function releases the mutex lock, allowing other tasks or threads to acquire it.
     * The mutex must have been previously acquired with acquireMutex() before calling this function.
     */
    void releaseMutex(void);

    /**
     * @brief Initializes the hardware serial communication interface.
     *
     * This function sets up the serial communication with the specified parameters,
     * including baud rate, pin assignments, signal inversion, and timeout settings.
     * It is essential to call this function before using the serial interface.
     *
     * @param serial Pointer to a HardwareSerial object used for serial communication.
     * @param baud The baud rate for serial communication (default: 115200).
     * @param rxPin The GPIO pin number designated for receiving data (default: -1 for auto).
     * @param txPin The GPIO pin number designated for transmitting data (default: -1 for auto).
     * @param config Configuration parameter for serial settings (default: SERIAL_8N1).
     * @param invert A boolean value indicating whether to invert the signal (default: false).
     * @param timeout_ms Timeout duration for reading data in milliseconds (default: 10000ms).
     * @param rxfifo_full_thrhd The threshold for the received FIFO buffer full condition (default: 112).
     */
    void begin(HardwareSerial *serial, unsigned long baud = 115200, int8_t rxPin = -1, int8_t txPin = -1,
               uint32_t config = SERIAL_8N1, bool invert = false, unsigned long timeout_ms = 10000UL,
               uint8_t rxfifo_full_thrhd = 112UL);

    /**
     * @brief Sends a data packet to the device at the specified position in the chain.
     *
     * This function sends a packet containing a command and data to the device
     * located at the specified position in the chain. The packet consists of a
     * command byte and the provided data buffer with its size.
     *
     * @param id Device position in the chain (starting from 1).
     * @param cmd Command byte to specify the operation.
     * @param buffer Pointer to the data buffer to be sent.
     * @param size Size of the data to be sent.
     */
    void sendPacket(uint16_t id, uint8_t cmd, const uint8_t *buffer, uint16_t size);

    /**
     * @brief Extracts a specific data packet based on ID and command.
     *
     * This function extracts the specified data packet from the buffer using the provided
     * device ID and command. It validates the packet's content but does not process it.
     * The packet is extracted and remains in the buffer for further processing.
     *
     * @param id Device position in the link (starting from 1).
     * @param cmd Command identifier for the specific packet to be extracted.
     *
     * @return Returns true if the packet is successfully extracted, false otherwise.
     */
    bool processPacket(uint16_t id, uint8_t cmd);

    /**
     * @brief Processes actively sent incoming data packets.
     *
     * This function handles the processing of data packets that are actively sent
     * by other devices. It checks the validity of the incoming packet and processes
     * it based on the packet's contents and command. After processing, the packet is
     * cleared from the buffer to prepare for the next incoming data.
     *
     * @return None
     */
    void processIncomingPacket(void);

    /**
     * @brief Checks the validity of the received data packet.
     *
     * This function checks whether the data packet in the given buffer is valid based on
     * the size and content. It helps to verify if the packet received is correctly formed
     * and ready for further processing.
     *
     * @param buffer Pointer to the data buffer containing the received packet.
     * @param size Size of the data packet in the buffer.
     *
     * @return Returns `true` if the packet is valid, `false` otherwise.
     */
    bool checkPacket(const uint8_t *buffer, uint16_t size);

    /**
     * @brief Waits for a specific data packet from the device at the specified position in the chain.
     *
     * This function waits for a data packet from the device at the given position in the chain,
     * based on the specified command byte. It will wait until the packet is received or the
     * timeout duration is exceeded.
     *
     * @param id Device position in the chain (starting from 1).
     * @param cmd Command byte to specify the expected data packet.
     * @param timeout Maximum time to wait for the packet in milliseconds.
     *
     * @return Returns `true` if the packet is received within the timeout period, `false` otherwise.
     */
    bool waitForData(uint16_t id, uint8_t cmd, uint32_t timeout);

    /**
     * @brief Sets the RGB value of the device at a specific position in the chain.
     *
     * This function sets the RGB color value of the device at the specified position in the chain.
     * The color is represented by an RGB structure containing the red, green, and blue components.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param rgb The RGB color value to be set (includes red, green, and blue components).
     * @param operationStatus Pointer to store the operation status after execution.
     * @param timeout The timeout duration for the operation in milliseconds (default: 100ms).
     *
     * @return Operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t setRGBValue(uint16_t id, rgb_color rgb, uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Gets the RGB value of the device at a specific position in the chain.
     *
     * This function retrieves the current RGB color value of the device at the specified position in the chain.
     * The color is returned as an RGB structure containing the red, green, and blue components.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param rgb Pointer to store the retrieved RGB color value (includes red, green, and blue components).
     * @param timeout The timeout duration for the operation in milliseconds (default: 100ms).
     *
     * @return Operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t getRGBValue(uint16_t id, rgb_color *rgb, unsigned long timeout = 100);

    /**
     * @brief Sets the RGB light brightness for the device at a specific position in the chain.
     *
     * This function sets the RGB light brightness of the device at the specified position in the chain.
     * It also allows the brightness setting to be saved to flash memory for persistence.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param rgbBrightness The brightness level of the RGB light (range: 0-100).
     * @param operationStatus Pointer to store the operation status after execution.
     * @param saveToFlash A flag indicating whether to save the setting to flash memory (SAVE_TO_FLASH_DISABLE: do not
     * save, SAVE_TO_FLASH_ENABLE: save to flash; default: SAVE_TO_FLASH_DISABLE).
     * @param timeout The timeout duration for the operation in milliseconds (default: 100ms).
     *
     * @return Operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t setRGBLight(uint16_t id, uint8_t rgbBrightness, uint8_t *operationStatus,
                               uint8_t saveToFlash = SAVE_TO_FLASH_DISABLE, unsigned long timeout = 100);

    /**
     * @brief Gets the RGB light brightness of the device at a specific position in the chain.
     *
     * This function retrieves the current RGB light brightness value of the device at the specified position in the
     * chain.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param rgbBrightness Pointer to store the retrieved RGB light brightness value (range: 0-100).
     * @param timeout The timeout duration for the operation in milliseconds (default: 100ms).
     *
     * @return Operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t getRGBLight(uint16_t id, uint8_t *rgbBrightness, unsigned long timeout = 100);

    /**
     * @brief Gets the bootloader version of the device at a specific position in the chain.
     *
     * This function retrieves the bootloader version of the device at the specified position in the chain.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param bootloaderVersion Pointer to store the retrieved bootloader version.
     * @param timeout The timeout duration for the operation in milliseconds (default: 100ms).
     *
     * @return Operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t getBootloaderVersion(uint16_t id, uint8_t *bootloaderVersion, unsigned long timeout = 100);

    /**
     * @brief Gets the firmware version of the device at a specific position in the chain.
     *
     * This function retrieves the firmware version of the device at the specified position in the chain.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param firmwareVersion Pointer to store the retrieved firmware version.
     * @param timeout The timeout duration for the operation in milliseconds (default: 100ms).
     *
     * @return Operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t getFirmwareVersion(uint16_t id, uint8_t *firmwareVersion, unsigned long timeout = 100);

    /**
     * @brief Gets the device type of the device at a specific position in the chain.
     *
     * This function retrieves the type of the device at the specified position in the chain.
     *
     * @param id The position of the device in the chain (starting from 1).
     * @param deviceType Pointer to store the retrieved device type.
     * @param timeout The timeout duration for the operation in milliseconds (default: 100ms).
     *
     * @return Operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t getDeviceType(uint16_t id, uint16_t *deviceType, unsigned long timeout = 100);

    /**
     * @brief Gets the total number of devices in the chain.
     *
     * This function retrieves the total number of devices connected in the chain.
     *
     * @param deviceNum Pointer to store the number of devices in the chain.
     * @param timeout The timeout duration for the operation in milliseconds (default: 100ms).
     *
     * @return Operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t getDeviceNum(uint16_t *deviceNum, unsigned long timeout = 100);

    /**
     * @brief Checks if the device is connected.
     *
     * This function checks the connection status of the device by attempting to communicate
     * with it a specified number of times (maxRetries). It waits for a response within a
     * given timeout period. If the device responds successfully within the timeout and retry
     * limit, it is considered connected.
     *
     * @param maxRetries The maximum number of retry attempts to check the device connection (default is 1).
     * @param timeout The timeout duration in milliseconds for each retry (default is 10ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t isDeviceConnected(uint8_t maxRetries = 1, unsigned long timeout = 10);

    /**
     * @brief Retrieves the list of connected devices.
     *
     * This function retrieves a list of all connected devices in the chain. It stores the list
     * of devices in the provided device_list_t structure, including device IDs and types.
     *
     * @param list Pointer to a device_list_t structure where the list of devices will be stored.
     * @param timeout The timeout duration in milliseconds for retrieving the device list (default is 200ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t getDeviceList(device_list_t *list, unsigned long timeout = 200);

    /**
     * @brief Retrieves the number of recorded 'Enum Please' packets.
     *
     * This function returns the number of 'Enum Please' packets that have been previously
     * recorded in the system. The number represents how many such packets are available
     * for further processing or action.
     *
     * @return The number of recorded 'Enum Please' packets.
     */
    uint16_t getEnumPleaseNum(void);

    /**
     * @brief Retrieves the recorded key buffer data.
     *
     * This function retrieves the key buffer data that has been recorded. The retrieved data
     * is stored in the provided output buffer, and the length of the data is returned through
     * the provided length parameter.
     *
     * @param outBuffer Pointer to the buffer where the recorded key data will be stored.
     * @param length Pointer to the variable that will store the length of the recorded key buffer.
     */
    void getKeyBuffer(uint16_t *outBuffer, uint16_t *length);

private:
    // Serial port instance
    HardwareSerial *serialPort;  // Instance of the hardware serial port used for communication.

    // Receive data buffer
    uint8_t receiveBuffer[RECEIVE_BUFFER_SIZE] = {0};  // Buffer used for storing received data.
    uint16_t receiveBufferSize                 = 0;    // Length of the data stored in the receive buffer.

    // Send data buffer
    uint8_t sendBuffer[SEND_BUFFER_SIZE] = {0};  // Buffer used for storing data to be sent.
    uint16_t sendBufferSize              = 0;    // Length of the data stored in the send buffer.

    // Key recording buffer
    uint16_t keyBuffer[KEY_BUFFER_SIZE] = {0};  // Buffer for storing recorded key data.
    uint16_t keyBufferSize              = 0;    // Length of the recorded key data stored in the key buffer.

    // Enum request count
    uint16_t enumPleaseCount = 0;  // The count of 'Enum Please' requests recorded.

    void receive();
    bool available();
    bool processPacketData(uint16_t id = 0, uint8_t cmd = 0);
    /**
     * @brief Calculates the CRC for the given data.
     *
     * This function calculates the CRC value for the provided data buffer. The CRC is used to check the integrity
     * of the data, ensuring that it has not been corrupted during transmission.
     *
     * @param buffer Pointer to the data buffer for which CRC needs to be calculated.
     * @param size Size of the data buffer.
     *
     * @return The calculated CRC value.
     */
    uint8_t calculateCRC(const uint8_t *buffer, uint16_t size);

    /**
     * @brief Checks the validity of the packet header.
     *
     * This function verifies if the packet header is correct by checking the given buffer's header section.
     * It ensures the packet follows the expected header format.
     *
     * @param buffer Pointer to the buffer containing the data to be checked.
     * @param size Size of the data buffer.
     *
     * @return True if the header is valid, false otherwise.
     */
    bool checkPacketHeader(const uint8_t *buffer, uint16_t size);

    /**
     * @brief Checks the validity of the packet tail.
     *
     * This function verifies if the packet tail is correct by checking the given buffer's tail section.
     * It ensures the packet ends with the expected tail format.
     *
     * @param buffer Pointer to the buffer containing the data to be checked.
     * @param size Size of the data buffer.
     *
     * @return True if the tail is valid, false otherwise.
     */
    bool checkPacketTail(const uint8_t *buffer, uint16_t size);

    /**
     * @brief Verifies the packet length.
     *
     * This function checks if the packet length is correct by comparing the actual length in the buffer
     * with the expected length based on the packet format.
     *
     * @param buffer Pointer to the buffer containing the data to be checked.
     * @param size Size of the data buffer.
     *
     * @return True if the packet length is valid, false otherwise.
     */
    bool checkPacketLength(const uint8_t *buffer, uint16_t size);

    /**
     * @brief Checks the CRC of the packet.
     *
     * This function validates the integrity of the packet by calculating and verifying its CRC
     *
     * @param buffer Pointer to the buffer containing the data to be checked.
     * @param size Size of the data buffer.
     *
     * @return True if the CRC is valid, false otherwise.
     */
    bool checkCRC(const uint8_t *buffer, uint16_t size);
};

#endif  // CHAIN_COMMON_H