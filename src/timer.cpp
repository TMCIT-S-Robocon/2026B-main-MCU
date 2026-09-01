#include "xc.h"
#include "timer.hpp"
#include "../src/solenoid_kyou.hpp"
#include "../src/neopixel.hpp"
//#include "../src/config/default/peripheral/qei/plib_qei1.h"
//#include "../src/config/default/peripheral/qei/plib_qei2.h"
//#include "../src/config/default/peripheral/qei/plib_qei3.h"
//#include <math.h>
//#define D_t 0.005f // 制御周期(s) 5ms

volatile uint16_t pair_timer[6] = {0};
volatile uint16_t delay_timer[6] = {0};

const uint8_t first_solenoid[6] = {1, 3, 5, 7, 9, 11};
const uint8_t second_solenoid[6] = {2, 4, 6, 8, 10, 12};

void solenoid_syasyutu(uint8_t pair){
    if(pair >= 6) return;
    
    uint8_t s1 = first_solenoid[pair];
    solenoid_kyou_individual(s1, true);
    // 1000ms
    pair_timer[pair] = SOLENOID_OFF_COUNT;
    // 230ms後にホールドON
    delay_timer[pair] = SOLENOID_DELAY_COUNT;
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
    
    // neopixel
    neopixel_timer_update();
}