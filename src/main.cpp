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
#include "../src/neopixel.hpp"
#include "../src/bno.hpp"
#include "../src/pid.hpp"
#include "../src/timer.hpp"

#define CCLK            (120000000L)            // system clock
#define PBCLK           (CCLK / 2)              // peripheral bus clock
#define SAMPLE_RATE     5000
#define pi 3.14159265358979323846264338327950288
#define rd (180.0/pi) //radian to degree

// IK
Chassis<Omni_4> Omni4_wheel;
// can device
Shotacon controller(&CAN4);
NEWHZWMD MD1(&CAN4, 0x513), MD2(&CAN4, 0x525), MD3(&CAN4, 0x524), MD4(&CAN4, 0x517);

// bno
BNO055 bno;
// pid 567.6
Omni4PID pid(0.06f, 0.567f, 2048.0f*4.0f); // 車輪半径[m], 中心-車輪距離[m], CPR

void init();
void debug_pid();

float angle = 0.0;
float yaw = 0.0;
float vx = 0.0;
float vy = 0.0;
float wz = 0.0;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void ){
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    D1_Set();
    
    init();
    
    // 左前と左後は前進するとデクリメント
    pid.set_encoder_sign(0, -1);
    pid.set_encoder_sign(1, -1);
    pid.set_encoder_sign(2, -1);
    pid.set_encoder_sign(3, -1);
    
    // PID番号は MD1, MD2, MD3, MD4 の順
//    pid.set_wheel_pid(0, 2.0f, 0.0f, 0.0f);  // MD1: 0x513, QEI4 左前
//    pid.set_wheel_pid(1, 3.0f, 0.0f, 0.0f);  // MD2: 0x525, QEI3 左後
//    pid.set_wheel_pid(2, 3.0f, 0.0f, 0.0f);  // MD3: 0x524, QEI2
//    pid.set_wheel_pid(3, 2.6f, 0.0f, 0.0f);  // MD4: 0x517, QEI1
    
//    pid.set_yaw_pid(2.0f, 0.0f, 0.0f);
//    pid.set_yaw_hold_deadband(0.08f);
    
    init_neopixel();
    solenoid_kyou_all(false);
    solenoid_kyou_individual(9, false);
    solenoid_kyou_individual(10, false);
    solenoid_kyou_individual(11, false);
    solenoid_kyou_individual(12, false);
        
    // BNO055初期化
    if(bno.begin()){
        D3_Set();
//        while(1);
    }

    while(1){
        Omni4_wheel.calc(controller.data.Lstick.theta,controller.data.Lstick.r*95.0,-4.0*controller.data.Rstick.x);
        
//        controller.readCAN();
//
////        if (bno.read_quaternion_yaw(&yaw)) {
////            pid.set_yaw(yaw, true);
////        } else {
////            pid.set_yaw(0.0f, false); // BNO異常時はヨー保持を止める
////        }
//        
//        vx = controller.data.Lstick.x / 7.0f * 0.5f;
//        vy = controller.data.Lstick.y / 7.0f * 0.5f;
//        wz = -controller.data.Rstick.x / 7.0f * 1.5f;
//        pid.set_velocity(vx, vy, wz);
//
////        debug_pid();
//        
//        MD1.Motors[0] = pid.get_output(0);
//        MD2.Motors[0] = pid.get_output(1);
//        MD3.Motors[0] = pid.get_output(2);
//        MD4.Motors[0] = pid.get_output(3);
        
        
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
        if(controller.data.L1){ // 射出            
            if(controller.data.F1){
                solenoid_syasyutu(0);
            } else if(controller.data.F2){
                solenoid_syasyutu(1);
            } else if(controller.data.F3){
                solenoid_syasyutu(2);
            } else if(controller.data.F4){
                solenoid_syasyutu(3);
            } else if(controller.data.F5){
                solenoid_syasyutu(4);
            } else if(controller.data.F6){
                solenoid_syasyutu(5);
            }
        } else if(controller.data.R1){ // 戻す
            if(controller.data.F1){ // 左上
                solenoid_kyou_individual(1, false);
                solenoid_kyou_individual(2, false);
            } else if(controller.data.F2){ // 左中
                solenoid_kyou_individual(3, false);
                solenoid_kyou_individual(4, false);
            } else if(controller.data.F3){ // 左下
                solenoid_kyou_individual(5, false);
                solenoid_kyou_individual(6, false);
            } else if(controller.data.F4){ // 右上
                solenoid_kyou_individual(7, false);
                solenoid_kyou_individual(8, false);
            } else if(controller.data.F5){
                solenoid_kyou_individual(9, false);
                solenoid_kyou_individual(10, false);
            } else if(controller.data.F6){ // 右下
                solenoid_kyou_individual(11, false);
                solenoid_kyou_individual(12, false);
            }
        } else if(controller.data.L2){ // 掴むところだけ開ける
            if(controller.data.F1){ // 左上
                solenoid_kyou_individual(2, false);
            } else if(controller.data.F2){ // 左中
                solenoid_kyou_individual(4, false);
            } else if(controller.data.F3){ // 左下
                solenoid_kyou_individual(6, false);
            } else if(controller.data.F4){ // 右上
                solenoid_kyou_individual(8, false);
            } else if(controller.data.F5){ // 右中
                solenoid_kyou_individual(10, false);
            } else if(controller.data.F6){ // 右下
                solenoid_kyou_individual(12, false);
            }
        } else if(controller.data.R2){ // 掴むところだけ閉める
            if(controller.data.F1){ // 左上
                solenoid_kyou_individual(2, true);
            } else if(controller.data.F2){ // 左中
                solenoid_kyou_individual(4, true);
            } else if(controller.data.F3){ // 左下
                solenoid_kyou_individual(6, true);
            } else if(controller.data.F4){ // 右上
                solenoid_kyou_individual(8, true);
            } else if(controller.data.F5){ // 右中
                solenoid_kyou_individual(10, true);
            } else if(controller.data.F6){ // 右下
                solenoid_kyou_individual(12, true);
            }
        }
        
        if(controller.data.U){ // 射出状態(初期状態)で保持
//            first_solenoid_init();
            if(controller.data.F1){ // 左上
                solenoid_kyou_individual(1, true);
            } else if(controller.data.F2){ // 左中
                solenoid_kyou_individual(3, true);
            } else if(controller.data.F3){ // 左下
                solenoid_kyou_individual(5, true);
            } else if(controller.data.F4){ // 右上
                solenoid_kyou_individual(7, true);
            } else if(controller.data.F5){ // 右中
                solenoid_kyou_individual(9, true);
            } else if(controller.data.F6){ // 右下
                solenoid_kyou_individual(11, true);
            }
        } else if(controller.data.X){ // 射出前の状態で保持
//            first_solenoid_before_syasyutu();
            if(controller.data.F1){ // 左上
                solenoid_kyou_individual(1, false);
            } else if(controller.data.F2){ // 左中
                solenoid_kyou_individual(3, false);
            } else if(controller.data.F3){ // 左下
                solenoid_kyou_individual(5, false);
            } else if(controller.data.F4){ // 右上
                solenoid_kyou_individual(7, false);
            } else if(controller.data.F5){ // 右中
                solenoid_kyou_individual(9, false);
            } else if(controller.data.F6){ // 右下
                solenoid_kyou_individual(11, false);
            }
        }
        
        // 遠隔非常停止(B6)
        if(controller.data.L){
            if(controller.data.A){
                B6_Clear();
                A6_Clear();
            }
        }
        
        if(neopixel_is_ready()){
            neopixel();
        }
                
        D1_Toggle();
    };
    
    return ( EXIT_FAILURE );
}

void init(){
    B6_Set();
    A6_Set();
    Omni4_wheel.set_motor(0, MD1.GetMotor(0)).set_motor(1, MD2.GetMotor(0)).set_motor(2, MD3.GetMotor(0)).set_motor(3, MD4.GetMotor(0));
    CAN4.init();
    TMR2_CallbackRegister(tmr2_isr, 0);
    TMR3_CallbackRegister(tmr3_isr, 0);
    TMR2_Start();
    TMR3_Start();
    QEI1_Start();
    QEI2_Start();
    QEI3_Start();
    QEI4_Start();
//    WDT_Enable();
}

void debug_pid(){
    uint8_t buf[6] = {0};
    
    buf[0] = 0x80;
    memcpy(&buf[1], &yaw, sizeof(float));
    buf[5] = 0x7f;
    
    UART1_Write(buf, 6);
}

/*******************************************************************************
 End of File
*/