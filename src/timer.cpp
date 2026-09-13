#include "xc.h"
#include "timer.hpp"
#include "../src/solenoid_kyou.hpp"
#include "../src/neopixel.hpp"
#include "../src/qei.hpp"
#include "../src/pid.hpp"
#include "../src/config/default/peripheral/qei/plib_qei1.h"
#include "../src/config/default/peripheral/qei/plib_qei2.h"
#include "../src/config/default/peripheral/qei/plib_qei3.h"
#include "../src/config/default/peripheral/qei/plib_qei4.h"
#include <math.h>
#define D_t 0.005f // 制御周期(s) 5ms

extern Omni4PID pid;
extern float angle;

volatile uint16_t pair_timer[6] = {0};
volatile uint16_t delay_timer[6] = {0};
volatile uint16_t first_solenoid_timer = 0;
volatile uint8_t first_solenoid_index = 0;

const uint8_t first_solenoid[6] = {1, 3, 5, 7, 9, 11};
const uint8_t second_solenoid[6] = {2, 4, 6, 8, 10, 12};
const uint16_t solenoid_delay_time[6] = {
//    110,  // F1
//    170,  // F2
//    100,  // F3
//    100,  // F4
//    100,  // F5 70
//    80   // F6
    170,  // F1
    170,  // F2
    170,  // F3
    170,  // F4
    170,  // F5 70
    170   // F6
};

void solenoid_syasyutu(uint8_t pair){
    if(pair >= 6) return;

    uint8_t s1 = first_solenoid[pair];

    // 1個目をON
    solenoid_kyou_individual(s1, true);

    // 1個目をONしてから1000ms後にOFF
    pair_timer[pair] = SOLENOID_OFF_COUNT;

    // 機構ごとの時間後に2個目をON
    delay_timer[pair] =
        solenoid_delay_time[pair] / SOLENOID_TIMER_PERIOD_MS;
}

void tmr3_isr(uint32_t status, uintptr_t context){
    // 230ms
    for(int i = 0; i < 6; i++){
        if(delay_timer[i] > 0){
            delay_timer[i]--;

            if(delay_timer[i] == 0){
                uint8_t s2 = second_solenoid[i];
                solenoid_kyou_individual(s2, true);
            }
        }
        
        // 1000ms
        if(pair_timer[i] > 0){
            pair_timer[i]--;

            if(pair_timer[i] == 0){
                uint8_t s1 = first_solenoid[i];
                uint8_t s2 = second_solenoid[i];

                solenoid_kyou_individual(s1, false);
                solenoid_kyou_individual(s2, false);
            }
        }
    }
    
    // 1,3,5,7,9,11の電磁弁を200msずつ閉じていく
    if(first_solenoid_timer > 0){

        first_solenoid_timer--;

        if(first_solenoid_timer == 0){

            // 現在の電磁弁をOFF
            solenoid_kyou_individual(
                first_solenoid[first_solenoid_index],
                false
            );

            first_solenoid_index++;

            // まだ残っている
            if(first_solenoid_index < 6){

                // 次の電磁弁まで200ms
                first_solenoid_timer =
                    FIRST_SOLENOID_INTERVAL_COUNT;
            }
        }
    }
    
    // neopixel
    neopixel_timer_update();
}

void tmr2_isr(uint32_t status, uintptr_t context){
    pid.update(
//        (int32_t)get_position(3), // MD1
//        (int32_t)get_position(4), // MD2
//        (int32_t)get_position(2), // MD3
//        (int32_t)get_position(1)  // MD4
        get_position(3), // MD1
        get_position(4), // MD2
        get_position(2), // MD3
        get_position(1)  // MD4
    );
    angle = get_position(2);
}

void first_solenoid_init(void){ // 機構全て初期(射出後)状態
    // すでに動作中なら無視
    if(first_solenoid_timer != 0){
        return;
    }

    first_solenoid_index = 0;

    // 1番を即座にOFF
    solenoid_kyou_individual(first_solenoid[0], true);

    first_solenoid_index = 1;
    first_solenoid_timer = FIRST_SOLENOID_INTERVAL_COUNT;
}

void first_solenoid_before_syasyutu(void){ // 機構全て射出前状態
    // すでに動作中なら無視
    if(first_solenoid_timer != 0){
        return;
    }

    first_solenoid_index = 0;

    // 1番を即座にON
    solenoid_kyou_individual(first_solenoid[0], false);

    first_solenoid_index = 1;
    first_solenoid_timer = FIRST_SOLENOID_INTERVAL_COUNT;
}