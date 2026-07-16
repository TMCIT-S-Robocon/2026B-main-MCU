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

float get_angle(){
    uint32_t pos = QEI1_PositionGet();
    float angle = (float)pos * 360.0f / 8192.0f;

    angle = fmodf(angle, 360.0f);
    if(angle < 0) angle += 360.0f;

    return angle;
}

uint32_t get_position(){
    uint32_t position = QEI1_PositionGet();
    uint32_t angle = position * 360 / 8192;
    return angle;
}

// IK
Chassis<Omni_4> Omni4_wheel;

// can device
Shotacon controller(&CAN1);
NEWHZWMD MD1(&CAN1, 0x522), MD2(&CAN1, 0x521), MD3(&CAN1, 0x511), MD4(&CAN1, 0x514);

float pos = 0.0;

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
//    Omni4_wheel.set_motor(0, MD1.GetMotor(0)).set_motor(1, MD2.GetMotor(0)).set_motor(2, MD3.GetMotor(0)).set_motor(3, MD4.GetMotor(0));
    CAN1.init();
    QEI1_Start();
    OCMP4_Enable();
    OCMP4_CompareSecondaryValueSet(0);
    solenoid1_Clear();
    solenoid2_Clear();
    TMR2_Start();
    
    *MD1.GetMotor(0) = 50;
    while(1){
//        Omni4_wheel.calc(controller.data.Rstick.theta,controller.data.Rstick.r*50.0,-3*controller.data.Lstick.y);
//        MD1.Transmit(); // 0x522
//        __delay_ms(5);
//        MD2.Transmit();
//        __delay_ms(1);
//        MD3.Transmit();
//        __delay_ms(1);
//        MD4.Transmit();
//        __delay_ms(1);
        
//        pos = get_position();
//        printf("pos: %.2f\n", pos);
//        
//        MD1.Transmit();
//        
//        if(pos > 1440){
//            *MD1.GetMotor(0) = 0;
//        } else{
//            if(controller.data.B){
//                D2_Set();
//                D3_Clear();
//                *MD1.GetMotor(0) = 80;
//            } else if(controller.data.X){
//                D3_Set();
//                D2_Clear();
//                *MD1.GetMotor(0) = -80;
//            } else{
//                D2_Clear();
//                D3_Clear();
//                *MD1.GetMotor(0) = 0;
//            }
//        }
        
        
        
//        OCMP4_CompareSecondaryValueSet(1406);
//        __delay_ms(100);
//        OCMP4_CompareSecondaryValueSet(2250);
//        __delay_ms(100);
        
        
        // delay
        // 青木の射出機構：180ms
        // 古川先輩の射出機構：222ms???
        // 180~230ms 2msごとに5回射出
        if(controller.data.B && controller.data.R2){ // 射出
            D3_Set();
            solenoid1_Set();
            solenoid3_Clear();
            __delay_ms(170);
            solenoid2_Set();;
        } else if(controller.data.Y && controller.data.R2){
            solenoid2_Toggle(); // 掴むところだけ開け閉め
        } else{
            D3_Clear();
        }
        
        if(controller.data.A && controller.data.R2){ // 閉じる
            D4_Set();
            solenoid1_Clear();
            solenoid3_Set();
//            solenoid2_Clear();
        } else{
            D4_Clear();
        }
        
        
        
        D1_Toggle();
    }
    
    return ( EXIT_FAILURE );
};

/*******************************************************************************
 End of File
*/