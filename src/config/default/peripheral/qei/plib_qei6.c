/*******************************************************************************
  Quadrature Encoder Interface (QEI6) Peripheral Library (PLIB)

  Company:
    Microchip Technology Inc.

  File Name:
    plib_qei6.c

  Summary:
    QEI6 Source File

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
#include "device.h"
#include "plib_qei6.h"
#include "interrupts.h"

// *****************************************************************************

// *****************************************************************************
// Section: QEI6 Implementation
// *****************************************************************************
// *****************************************************************************


void QEI6_Initialize (void)
{

    /* QEI6CON register  */
    /*  CCM    = 0 */
    /*  GATEN  = 0 */
    /*  CNTPOL = 0 */
    /*  INTDIV = 0 */
    /*  IMV    = 0  */
    /*  PIMOD  = 0  */
    /*  QEISIDL = 0 */
    QEI6CON = 0x0;

    /* QEI6IOC register  */
    /*  QEAPOL    = 0 */
    /*  QEBPOL  = 0 */
    /*  IDXPOL = 0 */
    /*  HOMPOL = 0 */
    /*  SWPAB    = 0  */
    /*  OUTFNC  = 0  */
    /*  QFDIV   = 0   */
    /*  FLTREN  = 0   */
    QEI6IOC = 0x0;

    QEI6ICC = 0U;
    QEI6CMPL = 0U;

    /* QEI6STAT register  */
    /*  IDXIEN    = false */
    /*  HOMIEN  = false */
    /*  VELOVIEN = false */
    /*  POSOVIEN = false */
    /*  PCIIEN    = false  */
    /*  PCLEQIEN  = false    */
    /*  PCHEQIEN = false     */
    QEI6STAT = 0x0;

}


void QEI6_Start(void)
{
    /* Enable QEI channel */
    QEI6CON |= (uint32_t)_QEI6CON_QEIEN_MASK;
}

void QEI6_Stop(void)
{
    /* Disable QEI channel */
    QEI6CON &= ~(uint32_t)_QEI6CON_QEIEN_MASK;
}

uint32_t QEI6_PulseIntervalGet(void)
{
    return (INT6HLD);
}

void QEI6_PositionWindowSet(uint32_t high_threshold, uint32_t low_threshold)
{
    QEI6ICC  = high_threshold;
    QEI6CMPL = low_threshold;
}

void QEI6_PositionCountSet(uint32_t position_count)
{
    POS6CNT = position_count;
}

void QEI6_VelocityCountSet(uint32_t velocity_count)
{
    VEL6CNT = velocity_count;
}


