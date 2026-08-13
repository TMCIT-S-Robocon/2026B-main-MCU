/*******************************************************************************
  Quadrature Encoder Interface (QEI) Peripheral Library Interface Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_qei6.h

  Summary:
    QEI PLIB Header File

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef PLIB_QEI6_H
#define PLIB_QEI6_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "device.h"
#include "plib_qei_common.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END


// *****************************************************************************
// Section: Interface
// *****************************************************************************
// *****************************************************************************

// *************************** QEI6 API ***************************************/
// *****************************************************************************

void QEI6_Initialize (void);

void QEI6_Start(void);

void QEI6_Stop(void);

__STATIC_INLINE uint32_t QEI6_PositionGet(void)
{
    return (POS6CNT);
}

__STATIC_INLINE uint32_t QEI6_VelocityGet(void)
{
    return (VEL6CNT);
}

__STATIC_INLINE uint32_t QEI6_RevolutionsGet(void)
{
    return (INDX6CNT);
}

uint32_t QEI6_PulseIntervalGet(void);

void QEI6_PositionWindowSet(uint32_t high_threshold, uint32_t low_threshold);

void QEI6_PositionCountSet(uint32_t position_count);

void QEI6_VelocityCountSet(uint32_t velocity_count);



// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif

// DOM-IGNORE-END
#endif // PLIB_QEI6_H
