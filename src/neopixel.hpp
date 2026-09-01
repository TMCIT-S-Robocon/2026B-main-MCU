/*
 * File:   neopixel.hpp
 * Author: natsu217
 *
 * Created on August 20, 2026, 10:44 AM
 */

#ifndef NEOPIXEL_HPP
#define NEOPIXEL_HPP

#include <stdint.h>

#define NUM_LEDS 39

#define NEOPIXEL_TIMER_PERIOD_MS 5
#define NEOPIXEL_MOVE_TIME_MS    80
#define NEOPIXEL_MOVE_COUNT (NEOPIXEL_MOVE_TIME_MS / NEOPIXEL_TIMER_PERIOD_MS)

void init_neopixel();
void neopixel();
void neopixel_timer_update();
bool neopixel_is_ready();

//typedef union{
//    unsigned char bytes[3];
//
//    struct{
//        uint8_t bit0  : 1;
//        uint8_t bit1  : 1;
//        uint8_t bit2  : 1;
//        uint8_t bit3  : 1;
//        uint8_t bit4  : 1;
//        uint8_t bit5  : 1;
//        uint8_t bit6  : 1;
//        uint8_t bit7  : 1;
//        uint8_t bit8  : 1;
//        uint8_t bit9  : 1;
//        uint8_t bit10 : 1;
//        uint8_t bit11 : 1;
//        uint8_t bit12 : 1;
//        uint8_t bit13 : 1;
//        uint8_t bit14 : 1;
//        uint8_t bit15 : 1;
//        uint8_t bit16 : 1;
//        uint8_t bit17 : 1;
//        uint8_t bit18 : 1;
//        uint8_t bit19 : 1;
//        uint8_t bit20 : 1;
//        uint8_t bit21 : 1;
//        uint8_t bit22 : 1;
//        uint8_t bit23 : 1;
//        uint8_t bit24 : 1;
//        uint8_t bit25 : 1;
//        uint8_t bit26 : 1;
//        uint8_t bit27 : 1;
//        uint8_t bit28 : 1;
//        uint8_t bit29 : 1;
//        uint8_t bit30 : 1;
//        uint8_t bit31 : 1;
//        uint8_t bit32 : 1;
//        uint8_t bit33 : 1;
//        uint8_t bit34 : 1;
//    } bits;
//
//} hex_union;
typedef union{
    unsigned char bytes[3];

    struct{
        uint8_t bit0  : 1;
        uint8_t bit1  : 1;
        uint8_t bit2  : 1;
        uint8_t bit3  : 1;
        uint8_t bit4  : 1;
        uint8_t bit5  : 1;
        uint8_t bit6  : 1;
        uint8_t bit7  : 1;
        uint8_t bit8  : 1;
        uint8_t bit9  : 1;
        uint8_t bit10 : 1;
        uint8_t bit11 : 1;
        uint8_t bit12 : 1;
        uint8_t bit13 : 1;
        uint8_t bit14 : 1;
        uint8_t bit15 : 1;
        uint8_t bit16 : 1;
        uint8_t bit17 : 1;
        uint8_t bit18 : 1;
        uint8_t bit19 : 1;
        uint8_t bit20 : 1;
        uint8_t bit21 : 1;
        uint8_t bit22 : 1;
        uint8_t bit23 : 1;
    } bits;

} hex_union;

extern volatile hex_union data[NUM_LEDS];
extern uint8_t MoveCount;
extern uint8_t rainbow[56][3];
extern void (*p_func[])();

void send_bit0();
void send_bit1();
void init_neopixel();
void neopixel();
void neopixel_set_color(uint8_t r, uint8_t g, uint8_t b);


#endif /* NEOPIXEL_HPP */