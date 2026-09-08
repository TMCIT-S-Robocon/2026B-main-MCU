#include <math.h> // fmodf
#include "xc.h"
#include "qei.hpp"
#include "config/default/peripheral/qei/plib_qei1.h"
#include "config/default/peripheral/qei/plib_qei2.h"
#include "config/default/peripheral/qei/plib_qei3.h"
#include "config/default/peripheral/qei/plib_qei4.h"

float get_angle(uint8_t num){
    uint32_t pos = 0;
    switch(num){
        case 1:
            pos = QEI1_PositionGet();
            break;
        case 2:
            pos = QEI2_PositionGet();
            break;            
        case 3:
            pos = QEI3_PositionGet();
            break;
        case 4:
            pos = QEI4_PositionGet();
            break;
        default:
            break;
    }
    
    float angle = (float)pos * 360.0f / 8192.0f;

    angle = fmodf(angle, 360.0f);
    if(angle < 0) angle += 360.0f;

    return angle;
}

uint32_t get_position(uint8_t num){
    uint32_t pos = 0;
    switch(num){
        case 1:
            pos = QEI1_PositionGet();
            break;
        case 2:
            pos = QEI2_PositionGet();
            break;            
        case 3:
            pos = QEI3_PositionGet();
            break;
        case 4:
            pos = QEI4_PositionGet();
            break;
        default:
            break;
    }
    
    return pos;
}