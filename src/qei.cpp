#include <math.h> // fmodf
#include "xc.h"
#include "qei.hpp"
#include "config/default/peripheral/qei/plib_qei1.h"

float get_angle(){
    uint32_t pos = QEI3_PositionGet();
    float angle = (float)pos * 360.0f / 8192.0f;

    angle = fmodf(angle, 360.0f);
    if(angle < 0) angle += 360.0f;

    return angle;
}

uint32_t get_position(){
    uint32_t position = QEI3_PositionGet();
    uint32_t angle = position * 360 / 8192;
    return angle;
}