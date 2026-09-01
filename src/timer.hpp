/* 
 * File:   timer.hpp
 * Author: natsu217
 *
 * Created on August 24, 2026, 12:38 AM
 */

#ifndef TIMER_HPP
#define	TIMER_HPP

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"

#define SOLENOID_TIMER_PERIOD_MS 5
#define SOLENOID_OFF_TIME_MS     1000
#define SOLENOID_DELAY_TIME_MS   100
#define SOLENOID_OFF_COUNT (SOLENOID_OFF_TIME_MS / SOLENOID_TIMER_PERIOD_MS)
#define SOLENOID_DELAY_COUNT (SOLENOID_DELAY_TIME_MS / SOLENOID_TIMER_PERIOD_MS)

extern volatile uint16_t pair_timer[6];
extern volatile uint16_t delay_timer[6];

void tmr3_isr(uint32_t status, uintptr_t context);
void solenoid_syasyutu(uint8_t pair);

#endif	/* TIMER_HPP */

