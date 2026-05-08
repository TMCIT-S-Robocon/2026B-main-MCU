/*******************************************************************************
 *  File:        MD.hpp
 *  Module:      CAN Abstraction Layer - Motor Drivers
 *
 *  Summary:
 *    Motor Driver abstraction for various motor modules over CAN.
 *
 *  Description:
 *    This header defines a template-based base class `MD_base` that handles
 *    common motor operations such as stopping all motors, accessing individual
 *    motor values, and transmitting motor commands over a CAN bus.
 *
 *    Specific motor types are implemented as derived classes that implement
 *    `Transmit()` according to each device's data format and channel count:
 *      - KonnMD
 *      - NEWHZWMD
 *      - NEWHZWMD2Byte
 *      - RJ45MD1channel
 *      - RJ45MD8channel
 *
 *    The design ensures:
 *      1. CAN pointer and ID are common members in the base class.
 *      2. Motors array is template-based for compile-time safety.
 *      3. Transmit() implementation is device-specific.
 *      4. No automatic buffer resizing; the caller is responsible for tx.data allocation.
 *
 *  Attention:
 *    This file is part of taku-256's personal library or SAME library.
 *    "SAME" does not mean "similar"; in Japanese it means "shark".
 *    The actual pronunciation may differ from what you expect.
 *
 *******************************************************************************/

/*******************************************************************************
 *  Copyright (C) 2026 taku-256
 *
 *  License:
 *    This file is part of the SAME library.
 *    The license of this file follows the license of SAME library.
 *    Refer to the top-level LICENSE file for details.
 *
 *  This software is provided "AS IS", without warranty of any kind.
 *  The author shall not be held liable for any damages arising from the use
 *  of this software.
 *
 *******************************************************************************/

#ifndef SAME_DRIVER__MOTORDRIVER_HPP_
#define SAME_DRIVER__MOTORDRIVER_HPP_

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#if defined(__has_include)
#if __has_include("same_communication/can.hpp")
#include "same_communication/can.hpp"
#endif
#if __has_include("can.hpp")
#include "can.hpp"
#endif
#endif

#ifndef SAME_LIBRARY__CAN_HPP_
#error "This header should not be included directly. Include same_communication/can.hpp instead."
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Motor Driver Base class
// *****************************************************************************
// *****************************************************************************

template <size_t N>
class MD_base {
protected:
    CAN_base* CAN; /**< Pointer to the CAN interface used for transmission */
    uint32_t ID;   /**< CAN message ID assigned to this motor driver */

public:
    double Motors[N]; /**< Array storing motor control values. Size N. */

    /**
     * @brief Constructor initializes the CAN interface and motor array.
     * @param can Pointer to a valid CAN interface object
     * @param id CAN message ID to use for transmissions
     *
     * Initializes all motor values to 0.0.
     */
    MD_base(CAN_base* can, uint32_t id) : CAN(can), ID(id) {
        for (size_t i = 0; i < N; ++i) Motors[i] = 0.0;
    }

    /**
     * @brief Returns pointer to a specific motor value.
     * @param i Motor index (0-based)
     * @return Pointer to the motor value, or nullptr if index is out of range
     *
     * Allows external code to modify motor values directly.
     */
    double* GetMotor(size_t i) {
        if (i >= N) return nullptr;
        return &Motors[i];
    }

    /**
     * @brief Sets all motor values to zero.
     *
     * Useful for emergency stop or initialization.
     */
    void stopAll() {
        for (size_t i = 0; i < N; ++i) Motors[i] = 0.0;
    }

    /**
     * @brief Transmit current motor values over CAN bus.
     * @return Reference to the object itself
     *
     * Must be implemented by each derived class according to
     * its specific protocol (data length, scaling, byte order, etc.).
     */
    virtual MD_base& Transmit() = 0;
};

// *****************************************************************************
// *****************************************************************************
// Section: Specific Motor Driver Implementations
// *****************************************************************************
// *****************************************************************************

/**
 * @class MD_Konn
 * @brief 1-channel motor driver, 2-byte transmission protocol.
 */
class MD_Konn : public MD_base<1> {
public:
    MD_Konn(CAN_base* can, uint32_t id) : MD_base<1>(can, id) {}

    MD_base<1>& Transmit() override {
        CAN->tx.id = ID;
        CAN->tx.dlc = 2;
        uint32_t val = 5000 + 50 * Motors[0];
        CAN->tx.data[0] = val >> 8;
        CAN->tx.data[1] = val & 0xFF;
        CAN->Transmit();
        return *this;
    }
};

/**
 * @class NEWHZWMD
 * @brief 1-channel motor driver, 1-byte transmission protocol.
 */
class NEWHZWMD : public MD_base<1> {
public:
    NEWHZWMD(CAN_base* can, uint32_t id) : MD_base<1>(can, id) {}

    MD_base<1>& Transmit() override {
        CAN->tx.id = ID;
        CAN->tx.dlc = 1;
        CAN->tx.data[0] = 100 + Motors[0];
        CAN->Transmit();
        return *this;
    }
};

/**
 * @class NEWHZWMD2Byte
 * @brief 1-channel motor driver, 2-byte transmission with LSB first.
 */
class NEWHZWMD2Byte : public MD_base<1> {
public:
    NEWHZWMD2Byte(CAN_base* can, uint32_t id) : MD_base<1>(can, id) {}

    MD_base<1>& Transmit() override {
        CAN->tx.id = ID;
        CAN->tx.dlc = 2;
        uint32_t val = (5000 + 50 * Motors[0]);
        CAN->tx.data[0] = val & 0xFF;
        CAN->tx.data[1] = val >> 8;
        CAN->Transmit();
        return *this;
    }
};

/**
 * @class RJ45MD1channel
 * @brief 1-channel RJ45-based motor driver, 1-byte command.
 */
class RJ45MD1channel : public MD_base<1> {
public:
    RJ45MD1channel(CAN_base* can, uint32_t id) : MD_base<1>(can, id) {}

    MD_base<1>& Transmit() override {
        CAN->tx.id = ID;
        CAN->tx.dlc = 1;
        CAN->tx.data[0] = 100 + Motors[0];
        CAN->Transmit();
        return *this;
    }
};

/**
 * @class RJ45MD8channel
 * @brief 8-channel RJ45 motor driver, 1-byte per channel.
 */
class RJ45MD8channel : public MD_base<8> {
public:
    RJ45MD8channel(CAN_base* can, uint32_t id) : MD_base<8>(can, id) {}

    MD_base<8>& Transmit() override {
        CAN->tx.id = ID;
        CAN->tx.dlc = 8;
        for (uint8_t i = 0; i < 8; ++i) {
            CAN->tx.data[i] = 100 + Motors[i];
        }
        CAN->Transmit();
        return *this;
    }
};

#endif /*SAME_DRIVER__MOTORDRIVER_HPP_*/

/*******************************************************************************
 End of File
*/
