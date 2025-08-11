/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_PIR_HPP_
#define CHAIN_PIR_HPP_

#include <ChainCommon.hpp>

/**
 * @brief CHAIN_PIIR device type code.
 */
#define CHAIN_PIR_DEVICE_TYPE_CODE (0x0009)

typedef enum {
    CHAIN_PIR_NO_PERSON = 0x00, /**< Status indicating the PIR is not detecting any person. */
    CHAIN_PIR_PERSON = 0x01,    /**< Status indicating the PIR is detecting a person. */
} pir_detect_result_t;          /**< Enumeration for recording the PIR's current detect result of the pir device. */


typedef enum {
    CHAIN_PIR_REPORT_NO_PERSON = 0x0300, /**< Status indicating the PIR is not detecting any person. */
    CHAIN_PIR_REPORT_PERSON = 0x0301,    /**< Status indicating the PIR is detecting a person. */
} pir_detect_report_t;          /**< Enumeration for recording the PIR's current detect result of the pir device. */


/**
 * @brief Enumeration for CHAIN_SWITCH device commands.
 *
 * This enumeration defines command codes for various operations of the CHAIN_SWITCH device.
 */
typedef enum {
    CHAIN_PIR_GET_IR_STATUS           = 0x37, /**< Command to get the current IR induction status */
    CHAIN_PIR_SET_AUTO_SEND_IR_STATUS = 0xE1,    /**< Command to set the auto-send ir status */
    CHAIN_PIR_GET_AUTO_SEND_IR_STATUS = 0xE2,    /**< Command to get the auto-send ir status */
} CHAIN_PIR_CMD_T;                         /**< Command types for Chain_PIR device operations */

class ChainPIR : virtual public ChainCommon {
public:
    /**
     * @brief Retrieves the current slip's switch status of the Switch device at the specified position in
     * the chain.
     *
     * This function gets the current slip direction change status of the Switch device. It determines whether the
     * rotation direction is increasing or decreasing in the down-to-up direction.
     *
     * @param id The position of the Switch device in the chain (starting from 1).
     * @param ir_status Pointer to store the current slip's switch status. (1 for open, 0 for close).
     * @param timeout The timeout duration for the operation in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., `CHAIN_OK`, `CHAIN_TIMEOUT`, etc.).
     */
    chain_status_t getIRStatus(uint8_t id, pir_detect_result_t *ir_status, unsigned long timeout = 100);


    chain_status_t setPIRDetectTriggerMode(uint8_t id, chain_detect_mode_t auto_status, uint8_t* operationStatus, unsigned long timeout = 100);

    chain_status_t getPIRDetectTriggerMode(uint8_t id, chain_detect_mode_t* auto_status, unsigned long timeout = 100);

    /**
     * @brief Gets the Chain_Switch type code.
     *
     * This function returns the type code for the Chain_Switch device, used to identify the device type.
     *
     * @return Returns the Chain_Switch device type code.
     */
    uint16_t getPIRTypeCode(void);

    bool getPIRdetectReport(uint8_t id, pir_detect_report_t *triggerStatus);

private:
};

#endif  // CHAIN_PIR_HPP_