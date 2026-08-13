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
#include "../src/SAME_Library_header/can.hpp"
#include "../src/SAME_Library_header/chassis.hpp"
#include "../src/SAME_Library_header/MD.hpp"
#include "../src/SAME_Library_header/shotacon.hpp"
// other files
#include "../src/delay.hpp"
#include "../src/solenoid_kyou.hpp"
#include "../src/qei.hpp"

#define CCLK            (120000000L)            // system clock
#define PBCLK           (CCLK / 2)              // peripheral bus clock
#define SAMPLE_RATE     5000

// IK
Chassis<Omni_4> Omni4_wheel;
// can device
Shotacon controller(&CAN4);
NEWHZWMD MD1(&CAN4, 0x516), MD2(&CAN4, 0x525), MD3(&CAN4, 0x524), MD4(&CAN4, 0x517);

void init();

float angle = 0.0;
bool solenoid_bool = false;

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
    
//    init();
    Omni4_wheel.set_motor(0, MD1.GetMotor(0)).set_motor(1, MD2.GetMotor(0)).set_motor(2, MD3.GetMotor(0)).set_motor(3, MD4.GetMotor(0));
    CAN4.init();

    while(1){
        Omni4_wheel.calc(controller.data.Lstick.theta,controller.data.Lstick.r*80.0,-3*controller.data.Rstick.x);
        
        MD1.Transmit();
        __delay_ms(1);
        MD2.Transmit();
        __delay_ms(1);
        MD3.Transmit();
        __delay_ms(1);
        MD4.Transmit();
        __delay_ms(1);
        
        // delay
        // 青木の射出機構：180ms
        // 古川先輩の射出機構：222ms???
        // 180~230ms 2msごとに5回射出
        if(controller.data.B && controller.data.R2){ // 射出
            D3_Set();
            solenoid_kyou(1, true);
            __delay_ms(220);
            solenoid_kyou(2, true);
        } else if(controller.data.Y && controller.data.R2){
            solenoid_kyou(2, true); // 掴むところだけ開け閉め
        } else if(controller.data.X && controller.data.R2){
            solenoid_kyou(2, false);
        } else{
            D3_Clear();
        }
        
        if(controller.data.A && controller.data.R2){ // 閉じる
            D4_Set();
            solenoid_kyou(1, false);
        } else{
            D4_Clear();
        }
        
        
//        OCMP10_CompareSecondaryValueSet(27);
        
//        // RB7, RC13 圧力スイッチ(NPN)
//        if(GPIO_PinRead(GPIO_PIN_RB7)){ // out2
//            // 閾値の範囲外
//            D2_Clear();
//        } else{
//            // 閾値の範囲内
//            D2_Set();
//        }
//        if(GPIO_PinRead(GPIO_PIN_RC13)){ // out1
//            // 閾値の範囲外
//            D3_Clear();
//        } else{
//            // 閾値の範囲内
//            D3_Set();
//        }
        
        
//        solenoid_kyou(2, true);
//        __delay_ms(500);
//        solenoid_kyou(2, false);
        __delay_ms(5);
        D1_Toggle();
    };
    
    return ( EXIT_FAILURE );
}

void init(){
//    Omni3_wheel.set_motor(0, MD1.GetMotor(0)).set_motor(1, MD2.GetMotor(0)).set_motor(2, MD3.GetMotor(0));
//    CAN4.init();
//    Omni4_wheel.set_motor(0, MD1.GetMotor(0)).set_motor(1, MD2.GetMotor(0)).set_motor(2, MD3.GetMotor(0)).set_motor(3, MD4.GetMotor(0));
//    led1_Clear();
//    TMR2_Start();
//    QEI1_Start();
//    solenoid1_Clear();
//    solenoid2_Clear();
}

/*******************************************************************************
 End of File
*/