/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.cpp

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
// taku_256's library(SAME library)
#include "../Main_2026.X/can.hpp"
#include "../Main_2026.X/chassis.hpp"
#include "../Main_2026.X/MD.hpp"
#include "../Main_2026.X/shotacon.hpp"

#define CCLK            (120000000L)            // system clock
#define PBCLK           (CCLK / 2)              // peripheral bus clock
#define SAMPLE_RATE     5000
#define CCLK_US         (PBCLK / 1000 / 1000)   // used for micro second delay
#define CCLK_MS         (PBCLK / 1000)          // used for milli second delay

void __delay_us(unsigned int  d) 
{
    unsigned int delayCount, startTime;       

    startTime = _CP0_GET_COUNT();     
    delayCount = d * CCLK_US; 
    while((_CP0_GET_COUNT() - startTime) < delayCount);  
 }

void __delay_ms(unsigned int d){
    unsigned int delayCount, startTime;       

    startTime = _CP0_GET_COUNT();
    delayCount = d * CCLK_MS;   
    while((_CP0_GET_COUNT() - startTime) < delayCount);   
}

// IK
Chassis<Omni_4> Omni4_wheel;

// can device
Shotacon controller(&CAN1);
NEWHZWMD MD1(&CAN1, 0x513), MD2(&CAN1, 0x518), MD3(&CAN1, 0x516), MD4(&CAN1, 0x515);

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

#define pi 3.14159265358979323846264338327950288
#define rd (180.0/pi) //radian to degree

int main ( void ){
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    Omni4_wheel.set_motor(0, MD1.GetMotor(0)).set_motor(1, MD2.GetMotor(0)).set_motor(2, MD3.GetMotor(0)).set_motor(3, MD4.GetMotor(0));
    CAN1.init();
    while(1){
        Omni4_wheel.calc(controller.data.Rstick.theta,controller.data.Rstick.r*50.0,-3*controller.data.Lstick.y);
        MD1.Transmit();
        __delay_ms(1);
        MD2.Transmit();
        __delay_ms(1);
        MD3.Transmit();
        __delay_ms(1);
        MD4.Transmit();
        __delay_ms(1);
        
        LED1_Toggle();
    }
    
    return ( EXIT_FAILURE );
};

/*******************************************************************************
 End of File
*/