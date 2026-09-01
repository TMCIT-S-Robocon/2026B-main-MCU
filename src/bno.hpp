/* 
 * File:   bno.hpp
 * Author: natsu217
 *
 * Created on August 24, 2026, 12:28 AM
 */

#ifndef BNO_HPP_
#define	BNO_HPP_

#include <stdint.h>
#include <stdbool.h>

class BNO055{
private:
    static constexpr uint8_t ADDRESS = 0x28;
    static constexpr uint8_t CHIP_ID_REG  = 0x00;
    static constexpr uint8_t OPR_MODE_REG = 0x3D;
    static constexpr uint8_t UNIT_SEL_REG = 0x3B;
    static constexpr uint8_t CALIB_REG    = 0x35;
    static constexpr uint8_t QUATERNION_REG = 0x20;

    bool write_reg(uint8_t reg, uint8_t value);
    bool read_reg(uint8_t reg, uint8_t* buffer, uint8_t length);
    
public:
    BNO055();
    bool begin();
    bool check_chip_id();
    bool set_config_mode();
    bool set_unit();
    bool set_ndof_mode();
    bool is_calibrated();
    bool read_yaw(float* yaw_deg);
    bool read_quaternion_yaw(float* yaw_deg);
    float normalize_angle(float angle);
    bool initialized() const;
};


#endif	/* BNO_HPP */

