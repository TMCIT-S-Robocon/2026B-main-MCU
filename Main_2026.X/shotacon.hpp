/*******************************************************************************
 *  File:        shotacon.hpp
 *  Module:      CAN Abstraction Layer - Shota's Controller
 *
 *  Summary:
 *    CAN interface for receiving controller input data (sticks, buttons).
 *
 *  Description:
 *    This header defines a class that converts raw CAN messages into
 *    normalized controller state (L/R stick, buttons, etc.).
 *    Designed for chaining in a hardware-independent CAN abstraction layer.
 *
 *  Attention:
 *    This file is part of taku-256's personal library or SAME library.
 *    "SAME" does not mean "similar"; in Japanese it means "shark".
 *    The actual pronunciation may differ from what you expect.
 *
 *******************************************************************************/

/*******************************************************************************
 *  Copyright (C) 2025 taku-256
 *
 *  License:
 *    This file is part of the SAME library.
 *    The license of this file follows the license of SAME library.
 *    Refer to the top-level LICENSE file for details.
 *
 *  This software is provided "AS IS", without warranty of any kind.
 *  The author shall not be held liable for any damages arising from the use
 *  of this software.
 ******************************************************************************/

#ifndef SAME_DRIVER__SHOTACON_HPP_
#define SAME_DRIVER__SHOTACON_HPP_

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <math.h>

#include <vector>
#if defined(__has_include)
#if __has_include("same_communication/can.hpp")
#include "same_communication/can.hpp"
#endif
#if __has_include("definitions.h")
#include "definitions.h"
#endif
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Shotacon base class
// *****************************************************************************
// *****************************************************************************

#define sdi_pi 3.14159265358979323846264338327950288
#define sdi_rd (180 / sdi_pi)  // radian to degree

#define sc_min(x, y) ((x) < (y) ? (x) : (y))
#define sc_max(x, y) ((x) > (y) ? (x) : (y))
#define sc_clamp(x, y) ((x) > (y) ? (y) : ((x) < -(y) ? -(y) : (x)))  // max(-y,min(x,y))

typedef struct {
    union {
        uint8_t raw[8];
        struct {
            union {
                uint8_t byte0;
                struct {
                    unsigned char L : 1;
                    unsigned char U : 1;
                    unsigned char D : 1;
                    unsigned char R : 1;
                    unsigned char Y : 1;
                    unsigned char X : 1;
                    unsigned char B : 1;
                    unsigned char A : 1;
                };
            };

            union {
                uint8_t byte1;
                struct {
                    unsigned char L2 : 1;
                    unsigned char L1 : 1;
                    unsigned char F6 : 1;
                    unsigned char F5 : 1;
                    unsigned char F4 : 1;
                    unsigned char F3 : 1;
                    unsigned char F2 : 1;
                    unsigned char F1 : 1;
                };
            };

            union {
                uint8_t byte2;
                struct {
                    unsigned char : 6;
                    unsigned char R2 : 1;
                    unsigned char R1 : 1;
                };
            };

            union {
                uint8_t raw;
                struct {
                    unsigned char LSB : 4;
                    unsigned char MSB : 4;
                };
            } __stickL;

            union {
                uint8_t raw;
                struct {
                    unsigned char LSB : 4;
                    unsigned char MSB : 4;
                };
            } __stickR;

            uint8_t reserved0;
            uint8_t reserved1;
            uint8_t reserved2;
        };
    };
    struct {
        double x;
        double y;
        double r;
        double theta;
    } Lstick;
    struct {
        double x;
        double y;
        double r;
        double theta;
    } Rstick;
} shotacon_t;

template <class Derived>
class Shotacon_Base {
public:
    volatile shotacon_t data;

    Shotacon_Base(CAN_base* can) : CAN(can) {}

    Derived& readCAN() {
        if (CAN) {
            memcpy((void*)data.raw, CAN->rx.data.data(), 8);

            int lx = 8 - data.__stickL.MSB;
            int ly = 8 - data.__stickL.LSB;
            int rx = 8 - data.__stickR.MSB;
            int ry = 8 - data.__stickR.LSB;

            int mask = -swap_stick_flag, t;
            t = (lx ^ rx) & mask;
            lx ^= t;
            rx ^= t;
            t = (ly ^ ry) & mask;
            ly ^= t;
            ry ^= t;

            t = (lx ^ ly) & -swap_xy_flags[0];
            lx ^= t;
            ly ^= t;
            t = (rx ^ ry) & -swap_xy_flags[1];
            rx ^= t;
            ry ^= t;

            if (inverse_flags[0]) lx = -lx;
            if (inverse_flags[1]) ly = -ly;
            if (inverse_flags[2]) rx = -rx;
            if (inverse_flags[3]) ry = -ry;

            data.Lstick.x = lx;
            data.Lstick.y = ly;
            data.Rstick.x = rx;
            data.Rstick.y = ry;

            data.Lstick.r = sc_min(sqrt(lx * lx + ly * ly) / 7.0, 1.0);
            data.Rstick.r = sc_min(sqrt(rx * rx + ry * ry) / 7.0, 1.0);

            data.Lstick.theta = atan2(ly, lx);
            data.Rstick.theta = atan2(ry, rx);
        }
        return static_cast<Derived&>(*this);
    }

    Derived& setInverseLx(bool flag) {
        inverse_flags[0] = flag;
        return static_cast<Derived&>(*this);
    }
    Derived& setInverseLy(bool flag) {
        inverse_flags[1] = flag;
        return static_cast<Derived&>(*this);
    }
    Derived& setInverseRx(bool flag) {
        inverse_flags[2] = flag;
        return static_cast<Derived&>(*this);
    }
    Derived& setInverseRy(bool flag) {
        inverse_flags[3] = flag;
        return static_cast<Derived&>(*this);
    }

    Derived& setSwapXY_L(bool flag) {
        swap_xy_flags[0] = flag;
        return static_cast<Derived&>(*this);
    }
    Derived& setSwapXY_R(bool flag) {
        swap_xy_flags[1] = flag;
        return static_cast<Derived&>(*this);
    }

    Derived& setSwapStick(bool flag) {
        swap_stick_flag = flag;
        return static_cast<Derived&>(*this);
    }

    Derived& setInverse(bool lx, bool ly, bool rx, bool ry) {
        inverse_flags[0] = lx;
        inverse_flags[1] = ly;
        inverse_flags[2] = rx;
        inverse_flags[3] = ry;
        return static_cast<Derived&>(*this);
    }

    Derived& setSwapXY(bool l, bool r) {
        swap_xy_flags[0] = l;
        swap_xy_flags[1] = r;
        return static_cast<Derived&>(*this);
    }

protected:
    bool inverse_flags[4] = {false, false, false, false};
    bool swap_xy_flags[2] = {false, false};
    bool swap_stick_flag = false;
    CAN_base* CAN = nullptr;
};

class Shotacon : public Shotacon_Base<Shotacon> {
public:
    Shotacon(CAN_base* can) : Shotacon_Base(can) {}
};

#endif /*SAME_DRIVER__SHOTACON_HPP_*/

/*******************************************************************************
 End of File
*/