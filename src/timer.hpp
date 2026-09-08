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
#define SOLENOID_OFF_COUNT (SOLENOID_OFF_TIME_MS / SOLENOID_TIMER_PERIOD_MS)
#define FIRST_SOLENOID_INTERVAL_MS 200
#define FIRST_SOLENOID_INTERVAL_COUNT (FIRST_SOLENOID_INTERVAL_MS / SOLENOID_TIMER_PERIOD_MS)

extern volatile uint16_t pair_timer[6];
extern volatile uint16_t delay_timer[6];
extern volatile uint16_t first_solenoid_timer;
extern volatile uint8_t first_solenoid_index;

void tmr3_isr(uint32_t status, uintptr_t context);
void tmr2_isr(uint32_t status, uintptr_t context);
void solenoid_syasyutu(uint8_t pair);
void first_solenoid_close_start(void);

#endif	/* TIMER_HPP */

