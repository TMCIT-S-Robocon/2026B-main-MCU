// -*- C++ -*-
/* 
 * File:   delay.hpp
 * Author: natsu217
 *
 * Created on August 5, 2026, 4:07 PM
 */

#ifndef DELAY_HPP
#define	DELAY_HPP

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"

#define CCLK            (120000000L)            // system clock
#define PBCLK           (CCLK / 2)              // peripheral bus clock
#define CCLK_US         (PBCLK / 1000 / 1000)   // used for micro second delay
#define CCLK_MS         (PBCLK / 1000)          // used for milli second delay

void __delay_us(unsigned int d);
void __delay_ms(unsigned int d);

#endif	/* DELAY_HPP */

