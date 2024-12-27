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
 * @brief Maximum size of the receive buffer.
 */
#define RECEIVE_BUFFER_SIZE (1024)

/**
 * @brief Maximum size of the send buffer.
 */
#define SEND_BUFFER_SIZE (256)

/**
 * @brief Maximum buffer size for command storage.
 */
#define CMD_BUFFER_SIZE (256)

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
 * @brief Minimum size of the packet.
 */
#define PACK_SIZE_MIN (0x09)

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
 */
#define RGB_MAX_BRIGHTNESS (100)

/**
 * @brief Maximum number of active packets that can be recorded.
 */
#define ACTIVE_PACKET_RECORD_SIZE (32)

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
} device_info_t;

/**
 * @brief List of devices structure.
 */
typedef struct {
    uint16_t count;         /**< Number of devices. */
    device_info_t *devices; /**< Array of devices. */
} device_list_t;

/**
 * @brief RGB color structure.
 */
typedef struct {
    uint8_t R; /**< Red component of the color. */
    uint8_t G; /**< Green component of the color. */
    uint8_t B; /**< Blue component of the color. */
} rgb_color_t;

/**
 * @brief Structure to store ID and type information of a record.
 *
 * This structure contains the ID and type of a record, not the record content.
 */
typedef struct {
    uint8_t id;    /**< ID of the record. */
    uint16_t type; /**< Type of the record. */
} record_info_t;

/**
 * @brief Structure representing a node in the linked list of record info.
 *
 * This node stores record info (ID and type) and a pointer to the next node.
 */
typedef struct node {
    record_info_t data; /**< Data (ID and type) of the record. */
    struct node *next;  /**< Pointer to the next node in the list. */
} node_t;

/**
 * @brief Structure for the list of record info.
 *
 * This structure represents a linked list of record info, with a pointer to the head node
 * and the count of nodes in the list.
 */
typedef struct {
    node_t *head;  /**< Pointer to the first node in the list (head of the list). */
    uint8_t count; /**< Number of nodes in the list. */
} record_list_t;

class ChainCommon {
public:
    void debugPrint(void);
    // Command buffer
    uint8_t cmdBuffer[CMD_BUFFER_SIZE] = {0};  // Buffer used to store commands that are to be sent.
    uint16_t cmdBufferSize             = 0;    // Current size of the command buffer (number of commands stored).

    // Returned command buffer
    uint8_t returnPacket[CMD_BUFFER_SIZE] = {0};  // Buffer used to store the returned command data after execution.
    uint16_t returnPacketSize = 0;  // Current size of the returned command buffer (number of returned data bytes).

    record_list_t recordList = {
        NULL, 0};  // Initialize an empty record_list_t with the head pointer set to NULL and count set to 0

    /**
     * @brief Adds a new record to the linked list.
     *
     * This function uses tail insertion to add a new record at the end of the list.
     *
     * @param list A pointer to the record_list_t.
     * @param info The data to be added, of type record_info_t.
     *
     * @return Returns true if the record was successfully added, otherwise returns false.
     */
    bool addRecord(record_list_t *list, record_info_t info);

    /**
     * @brief Finds and deletes a node with the specified ID in the list, and returns its data.
     *
     * This function traverses the list to find the node with the specified ID. If found, it deletes the node
     * from the list and stores the node's data in the result pointer.
     *
     * @param list A pointer to the record_list_t.
     * @param id The ID of the node to find and delete.
     * @param result A pointer to store the found data.
     *
     * @return Returns true if the node was found and deleted, otherwise returns false.
     */
    bool findRecord(record_list_t *list, uint8_t id, record_info_t *result);

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
     * @brief Queries and processes data packets actively sent by slave devices.
     *
     * This function checks whether the buffer contains data packets actively sent
     * by slave devices. If such packets are found, it processes them accordingly.
     * After processing, the handled packets are cleared from the buffer to prepare
     * for new incoming data.
     *
     * @return None
     */
    void processIncomingData(void);

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
     * @brief Waits to receive the response data packet from the specified device.
     *
     * This function waits for a response data packet from the device at the specified position in the chain,
     * based on the specified command byte. It will wait until the matching packet is received or the timeout
     * duration is exceeded.
     *
     * @param id Device position in the chain (starting from 1).
     * @param cmd Command byte to specify the expected response packet.
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
    chain_status_t setRGBValue(uint16_t id, rgb_color_t rgb, uint8_t *operationStatus, unsigned long timeout = 100);

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
    chain_status_t getRGBValue(uint16_t id, rgb_color_t *rgb, unsigned long timeout = 100);

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
     * limit, it is considered connected, and the function returns `true`; otherwise, it returns `false`.
     *
     * @param maxRetries The maximum number of retry attempts to check the device connection (default is 1).
     * @param timeout The timeout duration in milliseconds for each retry (default is 10ms).
     *
     * @return Returns `true` if the device is successfully connected, otherwise returns `false`.
     */
    bool isDeviceConnected(uint8_t maxRetries = 1, unsigned long timeout = 10);

    /**
     * @brief Retrieves the list of connected devices.
     *
     * This function retrieves a list of all connected devices in the chain. It stores the list
     * of devices in the provided device_list_t structure, including device IDs and types.
     * If the device list is successfully retrieved, the function returns `true`; otherwise, it returns `false`.
     *
     * @param list Pointer to a device_list_t structure where the list of devices will be stored.
     * @param timeout The timeout duration in milliseconds for retrieving the device list (default is 200ms).
     *
     * @return Returns `true` if the device list is successfully retrieved, otherwise returns `false`.
     */
    bool getDeviceList(device_list_t *list, unsigned long timeout = 200);

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

private:
    // Serial port instance
    HardwareSerial *serialPort;  // Instance of the hardware serial port used for communication.

    // Receive data buffer
    uint8_t receiveBuffer[RECEIVE_BUFFER_SIZE] = {0};  // Buffer used for storing received data.
    uint16_t receiveBufferSize                 = 0;    // Length of the data stored in the receive buffer.

    // Send data buffer
    uint8_t sendBuffer[SEND_BUFFER_SIZE] = {0};  // Buffer used for storing data to be sent.
    uint16_t sendBufferSize              = 0;    // Length of the data stored in the send buffer.

    // Enum request count
    uint16_t enumPleaseCount = 0;  // The count of 'Enum Please' requests recorded.

    /**
     * @brief Reads data from the serial receive buffer within a timeout period.
     *
     * This function reads incoming data from the serial port into the
     * `receiveBuffer` until either the timeout (`TIMEOUT_MS`) expires or
     * the buffer (`RECEIVE_BUFFER_SIZE`) reaches its limit. If new data is
     * detected, the timeout counter resets to ensure all available data is read.
     *
     * @note This function assumes a non-blocking approach and relies on
     *       the `available` method to check for incoming data.
     */
    void readBuffer(void);

    /**
     * @brief Checks if data is available in the receive buffer.
     *
     * This function checks whether there is any data available to be read from the serial port.
     * It returns `true` if there is data in the buffer, otherwise returns `false`.
     *
     * @return `true` if data is available, `false` if no data is available.
     */
    bool available();

    /**
     * @brief Processes the data in the receive buffer and looks for complete packets.
     *
     * This function scans the receive buffer for packets starting with the header `0xAA 0x55`.
     * It checks if the packet is complete (i.e., the length and checksum are correct),
     * and if the packet matches the given `id` and `cmd`, it processes the packet accordingly.
     * The function handles different types of packets, including enumerating and active packets.
     *
     * @param id The device ID to match in the packet.
     * @param cmd The command code to match in the packet.
     *
     * @return `true` if a matching packet is found and processed, otherwise `false`.
     */
    bool processBufferData(uint16_t id = 0, uint8_t cmd = 0);

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
};

#endif  // CHAIN_COMMON_H