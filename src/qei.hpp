// -*- C++ -*-
/* 
 * File:   qei.hpp
 * Author: natsu217
 *
 * Created on August 10, 2026, 2:20 AM
 */

#ifndef QEI_HPP
#define	QEI_HPP

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"

float get_angle(uint8_t num);
uint32_t get_position(uint8_t num);

#endif	/* QEI_HPP */

