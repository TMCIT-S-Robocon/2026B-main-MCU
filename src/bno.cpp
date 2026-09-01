#include "bno.hpp"
#include "delay.hpp"
#include "config/default/definitions.h"
#include <math.h>

BNO055::BNO055(){
    
}

bool BNO055::write_reg(uint8_t reg, uint8_t value){
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = value;
    
    if(!I2C4_Write(ADDRESS, buf, 2)){
        return false;
    }
    while(I2C4_IsBusy());
    
    return true;
}

bool BNO055::read_reg(uint8_t reg, uint8_t* buffer, uint8_t length){
    if(!I2C4_WriteRead(ADDRESS, &reg, 1, buffer, length)){
        return false;
    }
    while(I2C4_IsBusy());
    
    return true;
}

bool BNO055::check_chip_id(){
    uint8_t id = 0;
    
    if(!read_reg(CHIP_ID_REG, &id, 1)){
        return false;
    }
    
    return id == 0xA0;
}

bool BNO055::set_config_mode(){
    // CONFIGMODE: 0000b -> 0x00
    if(!write_reg(OPR_MODE_REG, 0x00)){
        return false;
    }
    __delay_ms(20);
    
    return true;
}

bool BNO055::set_unit(){
    // Degrees, Dps, m/s^2 -> default
    // 0000b -> 0x00
    return write_reg(UNIT_SEL_REG, 0x00);
}

bool BNO055::set_ndof_mode(){
    // NDOF: 1100b -> 0x0C
    if(!write_reg(OPR_MODE_REG, 0x0C)){
        return false;
    }
    __delay_ms(20);
    
    return true;
}

bool BNO055::begin(){
    __delay_ms(700);
    
    if(!check_chip_id()){
        return false;
    }
    if(!set_config_mode()){
        return false;
    }
    if(!set_unit()){
        return false;
    }
    if(!set_ndof_mode()){
        return false;
    }
    
    return true;
}

bool BNO055::is_calibrated(){
    uint8_t value = 0;
    
    if(!read_reg(CALIB_REG, &value, 1)){
        return false;
    }
    
    uint8_t sys = (value >> 6) & 0x03;
//    uint8_t gyr = (value >> 4) & 0x03;
//    uint8_t acc = (value >> 2) & 0x03;
//    uint8_t mag = (value >> 0) & 0x03;
    
    return sys == 3;
}

bool BNO055::read_yaw(float* yaw_deg){
    if(yaw_deg == nullptr){
        return false;
    }
    
    uint8_t buf[2];
    
    if(!read_reg(0x1A, buf, 2)){
        return false;
    }
    
    int16_t raw = (int16_t)(((uint8_t)buf[1] << 8) | buf[0]);
    // Euler angle: 1degree = 16LSB
    *yaw_deg = (float)raw / 16.0f;
    
    return true;
}

bool BNO055::read_quaternion_yaw(float* yaw_deg){
    if(yaw_deg == nullptr){
        return false;
    }
    
    uint8_t buf[8];
    
    if(!read_reg(QUATERNION_REG, buf, 8)){
        return false;
    }
    
    int16_t qw_i = (int16_t)(buf[1] << 8) | buf[0];
    int16_t qx_i = (int16_t)(buf[3] << 8) | buf[2];
    int16_t qy_i = (int16_t)(buf[5] << 8) | buf[4];
    int16_t qz_i = (int16_t)(buf[7] << 8) | buf[6]; // (uint16_t)buf[n]にすべき?
    
    // 1 Quaternion (unit less) = 2^14 LSB
    float qw = (float)qw_i / 16384.0f;    
    float qx = (float)qx_i / 16384.0f;
    float qy = (float)qy_i / 16384.0f;
    float qz = (float)qz_i / 16384.0f;
    
    float yaw_rad = atan2f(
        2.0f * (qw*qz + qx*qy),
        1.0f - 2.0f * (qy*qy + qz*qz)
    );
    
//    yaw_rad = -yaw_rad; // 機体の方向に合わせて反転（必要であれば）
    
    *yaw_deg = yaw_rad * 180.0f / (float)M_PI;
    
    *yaw_deg = normalize_angle(*yaw_deg); // -180~180に正規化
    
    return true;
}

float BNO055::normalize_angle(float angle){
    while(angle > 180.0f) angle -= 360.0f;
    while(angle < -180.0f) angle += 360.0f;
    
    return angle;
}

bool BNO055::initialized() const{
    return true;
}