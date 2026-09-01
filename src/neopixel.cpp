/*
 * File:   neopixel.cpp
 * Author: natsu217
 *
 * Created on August 20, 2026, 10:44 AM
 */

#include "xc.h"
#include "neopixel.hpp"
#include "delay.hpp"

// B11

volatile hex_union data[NUM_LEDS];
uint8_t MoveCount = 0;
volatile uint16_t neopixel_timer = 0;

uint8_t rainbow[56][3] = {   
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    {0xE8,0xFA,0x00}, // yellow
    
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
//    {0xE8,0xFA,0x00}, // yellow
//    {0x00,0x00,0x00}, // white
    
//    {0x00,0xFF,0x00}, // red
//    {0x08,0xFF,0x00}, // orange
//    {0xE8,0xFA,0x00}, // yellow
//    {0xFF,0x00,0x00}, // green
//    {0xFF,0x00,0xFF}, // cyan
//    {0x00,0x00,0xFF}, // blue
//    {0x00,0xF0,0xF0}, // purple
//
//    {0x00,0xFF,0x00}, // red
//    {0x08,0xFF,0x00}, // orange
//    {0xE8,0xFA,0x00}, // yellow
//    {0xFF,0x00,0x00}, // green
//    {0xFF,0x00,0xFF}, // cyan
//    {0x00,0x00,0xFF}, // blue
//    {0x00,0xF0,0xF0}, // purple
//
//    {0x00,0xFF,0x00}, // red
//    {0x08,0xFF,0x00}, // orange
//    {0xE8,0xFA,0x00}, // yellow
//    {0xFF,0x00,0x00}, // green
//    {0xFF,0x00,0xFF}, // cyan
//    {0x00,0x00,0xFF}, // blue
//    {0x00,0xF0,0xF0}, // purple
//
//    {0x00,0xFF,0x00}, // red
//    {0x08,0xFF,0x00}, // orange
//    {0xE8,0xFA,0x00}, // yellow
//    {0xFF,0x00,0x00}, // green
//    {0xFF,0x00,0xFF}, // cyan
//    {0x00,0x00,0xFF}, // blue
//    {0x00,0xF0,0xF0}, // purple
//
//    {0x00,0xFF,0x00}, // red
//    {0x08,0xFF,0x00}, // orange
//    {0xE8,0xFA,0x00}, // yellow
//    {0xFF,0x00,0x00}, // green
//    {0xFF,0x00,0xFF}, // cyan
//    {0x00,0x00,0xFF}, // blue
//    {0x00,0xF0,0xF0}, // purple
//
//    {0x00,0xFF,0x00}, // red
//    {0x08,0xFF,0x00}, // orange
//    {0xE8,0xFA,0x00}, // yellow
//    {0xFF,0x00,0x00}, // green
//    {0xFF,0x00,0xFF}, // cyan
//    {0x00,0x00,0xFF}, // blue
//    {0x00,0xF0,0xF0}, // purple
//
//    {0x00,0xFF,0x00}, // red
//    {0x08,0xFF,0x00}, // orange
//    {0xE8,0xFA,0x00}, // yellow
//    {0xFF,0x00,0x00}, // green
//    {0xFF,0x00,0xFF}, // cyan
//    {0x00,0x00,0xFF}, // blue
//    {0x00,0xF0,0xF0}, // purple
//
//    {0x00,0xFF,0x00}, // red
//    {0x08,0xFF,0x00}, // orange
//    {0xE8,0xFA,0x00}, // yellow
//    {0xFF,0x00,0x00}, // green
//    {0xFF,0x00,0xFF}, // cyan
//    {0x00,0x00,0xFF}, // blue
//    {0x00,0xF0,0xF0}, // purple
};

void (*p_func[])() =
{
    send_bit0,
    send_bit1
};


// ============================================================
// Send "0" to WS2812
// ============================================================

void inline send_bit0(){
    LATBbits.LATB11 = 0;
    LATBbits.LATB11 = 0;
    LATBbits.LATB11 = 0;
    LATBbits.LATB11 = 1;
    LATBbits.LATB11 = 1;
    LATBbits.LATB11 = 1;
    LATBbits.LATB11 = 1;
    LATBbits.LATB11 = 1;
    LATBbits.LATB11 = 1;
    
    LATFbits.LATF1 = 0;
    LATFbits.LATF1 = 0;
    LATFbits.LATF1 = 0;
    LATFbits.LATF1 = 1;
    LATFbits.LATF1 = 1;
    LATFbits.LATF1 = 1;
    LATFbits.LATF1 = 1;
    LATFbits.LATF1 = 1;
    LATFbits.LATF1 = 1;
}

void inline send_bit1(){
    LATBbits.LATB11 = 0;
    LATBbits.LATB11 = 0;
    LATBbits.LATB11 = 0;
    LATBbits.LATB11 = 0;
    LATBbits.LATB11 = 0;
    LATBbits.LATB11 = 0;
    LATBbits.LATB11 = 1;
    LATBbits.LATB11 = 1;
    LATBbits.LATB11 = 1;
    LATBbits.LATB11 = 1;
    LATBbits.LATB11 = 1;
    LATBbits.LATB11 = 1;
    
    LATFbits.LATF1 = 0;
    LATFbits.LATF1 = 0;
    LATFbits.LATF1 = 0;
    LATFbits.LATF1 = 0;
    LATFbits.LATF1 = 0;
    LATFbits.LATF1 = 0;
    LATFbits.LATF1 = 1;
    LATFbits.LATF1 = 1;
    LATFbits.LATF1 = 1;
    LATFbits.LATF1 = 1;
    LATFbits.LATF1 = 1;
    LATFbits.LATF1 = 1;
}


// ============================================================
// Initialize NeoPixel
// ============================================================

void init_neopixel(){
    for (int i = 0; i < NUM_LEDS; i++){
//        data[i].bytes[0] = rainbow[i][0];
//        data[i].bytes[1] = rainbow[i][1];
//        data[i].bytes[2] = rainbow[i][2];
//        data[i].bytes[0] = 0xE8;
//        data[i].bytes[1] = 0xFA;
//        data[i].bytes[2] = 0x00;
    int index = (i + MoveCount) % 56;
    data[i].bytes[0] = rainbow[index][0];
    data[i].bytes[1] = rainbow[index][1];
    data[i].bytes[2] = rainbow[index][2];
    }

    // Set all PORTB,F pins as inputs first
    TRISB = 0xFFFF;
    TRISF = 0xFFFF;
    // RB11,RF1 = output
    TRISBbits.TRISB11 = 0;
    TRISFbits.TRISF1 = 0;
    // Initial output state
    LATBbits.LATB11 = 0;
    LATFbits.LATF1 = 0;
}

void neopixel(){
//    for (int k = 0; k < NUM_LEDS; k++){
//        p_func[data[k].bits.bit0]();
//        p_func[data[k].bits.bit1]();
//        p_func[data[k].bits.bit2]();
//        p_func[data[k].bits.bit3]();
//        p_func[data[k].bits.bit4]();
//        p_func[data[k].bits.bit5]();
//        p_func[data[k].bits.bit6]();
//        p_func[data[k].bits.bit7]();
//        p_func[data[k].bits.bit8]();
//        p_func[data[k].bits.bit9]();
//        p_func[data[k].bits.bit10]();
//        p_func[data[k].bits.bit11]();
//        p_func[data[k].bits.bit12]();
//        p_func[data[k].bits.bit13]();
//        p_func[data[k].bits.bit14]();
//        p_func[data[k].bits.bit15]();
//        p_func[data[k].bits.bit16]();
//        p_func[data[k].bits.bit17]();
//        p_func[data[k].bits.bit18]();
//        p_func[data[k].bits.bit19]();
//        p_func[data[k].bits.bit20]();
//        p_func[data[k].bits.bit21]();
//        p_func[data[k].bits.bit22]();
//        p_func[data[k].bits.bit23]();
//        p_func[data[k].bits.bit24]();
//        p_func[data[k].bits.bit25]();
//        p_func[data[k].bits.bit26]();
//        p_func[data[k].bits.bit27]();
//        p_func[data[k].bits.bit28]();
//        p_func[data[k].bits.bit29]();
//        p_func[data[k].bits.bit30]();
//        p_func[data[k].bits.bit31]();
//        p_func[data[k].bits.bit32]();
//        p_func[data[k].bits.bit33]();
//        p_func[data[k].bits.bit34]();
//    }
    
for (int k = 0; k < NUM_LEDS; k++) {

    for (int byte = 0; byte < 3; byte++) {

        for (int bit = 7; bit >= 0; bit--) {

            if (data[k].bytes[byte] & (1 << bit)) {
                send_bit1();
            } else {
                send_bit0();
            }
        }
    }
}
    
    for (int i = 0; i < NUM_LEDS; i++){
//        data[i].bytes[0] = rainbow[i + MoveCount][0];
//        data[i].bytes[1] = rainbow[i + MoveCount][1];
//        data[i].bytes[2] = rainbow[i + MoveCount][2];
//        data[i].bytes[0] = 0xE8;
//        data[i].bytes[1] = 0xFA;
//        data[i].bytes[2] = 0x00;
    int index = (i + MoveCount) % 56;
    data[i].bytes[0] = rainbow[index][0];
    data[i].bytes[1] = rainbow[index][1];
    data[i].bytes[2] = rainbow[index][2];
    }

//    if (MoveCount >= NUM_LEDS - 1){
//        MoveCount = 0;
//    }
//    
//    MoveCount++;
MoveCount++;

if (MoveCount >= 56){
    MoveCount = 0;
}

    // 80msタイマー開始
    neopixel_timer = NEOPIXEL_MOVE_COUNT;
}

void neopixel_timer_update(){
    if(neopixel_timer > 0){
        neopixel_timer--;
    }
}

bool neopixel_is_ready(){
    return (neopixel_timer == 0);
}

void neopixel_set_color(uint8_t r, uint8_t g, uint8_t b){
    // 全LEDを同じ色に設定
    for (int i = 0; i < NUM_LEDS; i++) {
        data[i].bytes[0] = r;
        data[i].bytes[1] = g;
        data[i].bytes[2] = b;
    }

    // WS2812へ送信
    for (int k = 0; k < NUM_LEDS; k++) {
        p_func[data[k].bits.bit0]();
        p_func[data[k].bits.bit1]();
        p_func[data[k].bits.bit2]();
        p_func[data[k].bits.bit3]();
        p_func[data[k].bits.bit4]();
        p_func[data[k].bits.bit5]();
        p_func[data[k].bits.bit6]();
        p_func[data[k].bits.bit7]();
        p_func[data[k].bits.bit8]();
        p_func[data[k].bits.bit9]();
        p_func[data[k].bits.bit10]();
        p_func[data[k].bits.bit11]();
        p_func[data[k].bits.bit12]();
        p_func[data[k].bits.bit13]();
        p_func[data[k].bits.bit14]();
        p_func[data[k].bits.bit15]();
        p_func[data[k].bits.bit16]();
        p_func[data[k].bits.bit17]();
        p_func[data[k].bits.bit18]();
        p_func[data[k].bits.bit19]();
        p_func[data[k].bits.bit20]();
        p_func[data[k].bits.bit21]();
        p_func[data[k].bits.bit22]();
        p_func[data[k].bits.bit23]();
//        p_func[data[k].bits.bit24]();
//        p_func[data[k].bits.bit25]();
//        p_func[data[k].bits.bit26]();
//        p_func[data[k].bits.bit27]();
//        p_func[data[k].bits.bit28]();
//        p_func[data[k].bits.bit29]();
//        p_func[data[k].bits.bit30]();
//        p_func[data[k].bits.bit31]();
//        p_func[data[k].bits.bit32]();
//        p_func[data[k].bits.bit33]();
//        p_func[data[k].bits.bit34]();
    }
}
