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
#define SOLENOID_TIMER_PERIOD_MS 5
#define SOLENOID_OFF_TIME_MS 1000
#define SOLENOID_TIMER_COUNT (SOLENOID_OFF_TIME_MS / SOLENOID_TIMER_PERIOD_MS)

// IK
Chassis<Omni_4> Omni4_wheel;
// can device
Shotacon controller(&CAN4);
NEWHZWMD MD1(&CAN4, 0x516), MD2(&CAN4, 0x525), MD3(&CAN4, 0x524), MD4(&CAN4, 0x517);

// pid
//Omni4PID pid(
//    0.05f,
//    0.17f,
//    2048*4
//);
// bno
BNO055 bno;

void init();
void debug_pid();

float angle = 0.0;
float yaw = 0.0;

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
    
    D1_Set();
    
    init();
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
//    // pidモーター方向
//    pid.set_motor_sign(0, 1);
//    pid.set_motor_sign(1, 1);
//    pid.set_motor_sign(2, 1);
//    pid.set_motor_sign(3, 1);
//    // 速度PID
//    pid.set_all_wheel_pid(
//        15.0f,
//        40.0f,
//        0.0f
//    );
//    // 姿勢PID
//    pid.set_yaw_pid(
//        0.5f,
//        0.0f,
//        0.0f
//    );
    

    while(1){
        Omni4_wheel.calc(controller.data.Lstick.theta,controller.data.Lstick.r*80.0,-3*controller.data.Rstick.x);
        
//        // コントローラー読み取り
//        controller.readCAN();
//        
//        // BNO055
//        if(bno.read_quaternion_yaw(&yaw)){
//            pid.set_yaw(yaw);
//        }
//        
//        debug_pid();
//        
//        // 操作入力
//        float vx = controller.data.Lstick.x / 7.0f * 0.5f;
//        float vy = controller.data.Lstick.y / 7.0f * 0.5f;
//        float wz = -controller.data.Rstick.x / 7.0f * 1.5f;
//        // PIDへ速度指令
//        pid.set_velocity(vx, vy, wz);
//        // PID出力をモーターへ
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
           
//        OCMP2_CompareSecondaryValueSet(1875); // GWS S35 STD
//        __delay_ms(1000);
//        OCMP2_CompareSecondaryValueSet(2344);
//        __delay_ms(1000);
        
        
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
        
        if(neopixel_is_ready()){
            neopixel();
        }
        
        D1_Toggle();
    };
    
    return ( EXIT_FAILURE );
}

void init(){
    Omni4_wheel.set_motor(0, MD1.GetMotor(0)).set_motor(1, MD2.GetMotor(0)).set_motor(2, MD3.GetMotor(0)).set_motor(3, MD4.GetMotor(0));
    CAN4.init();
    TMR3_CallbackRegister(tmr3_isr, 0);
    TMR2_Start();
    TMR3_Start();
    QEI1_Start();
    QEI2_Start();
    QEI3_Start();
    QEI4_Start();
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