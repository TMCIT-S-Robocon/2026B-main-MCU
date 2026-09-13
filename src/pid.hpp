/* 
 * File:   pid.hpp
 * Author: natsu217
 *
 * Created on August 24, 2026, 12:29 AM
 */

#ifndef PID_HPP_
#define PID_HPP_

#include <stdint.h>

// 4輪オムニ用の速度PID + 手動操縦時のヨー保持。
// update() は必ず一定周期のタイマ割り込みから呼ぶこと。
class Omni4PID {
public:
    static constexpr uint8_t WHEEL_COUNT = 4;

    struct WheelSpeed {
        float w0, w1, w2, w3; // [rad/s]
    };

    // wheelRadius: 車輪半径 [m]
    // rotationRadius: 機体中心から各車輪接地点までの有効距離 [m]
    // cpr: 車輪1回転あたりのエンコーダカウント数（逓倍後）
    Omni4PID(float wheelRadius, float rotationRadius, float cpr,
             float controlPeriod = 0.005f);

    // 操縦指令。vx, vy [m/s]、wz [rad/s]。毎回メインループから設定する。
    void set_velocity(float vx, float vy, float wz);
    void set_yaw(float yawDeg, bool valid = true);

    // 目標角度を明示的に指定したいときのみ使う。通常の手動操縦では不要。
    void set_target_yaw(float yawDeg);
    void set_yaw_hold_enabled(bool enabled);
    void set_yaw_hold_deadband(float wzDeadband);

    void set_wheel_pid(uint8_t wheel, float kp, float ki, float kd);
    void set_all_wheel_pid(float kp, float ki, float kd);
    // kS: 静止摩擦を越える固定出力、kV: 目標角速度[rad/s]あたりの出力。
    // どちらもモータ符号を反映する前の、正方向の正の値を渡す。
    void set_wheel_feedforward(uint8_t wheel, float kS, float kV);
    void set_yaw_pid(float kp, float ki, float kd);
    void set_motor_sign(uint8_t wheel, int8_t sign);
    void set_encoder_sign(uint8_t wheel, int8_t sign);

    // timer ISRで呼ぶ。countはQEIの累積カウントをそのまま渡す。
//    void update(int32_t enc0, int32_t enc1, int32_t enc2, int32_t enc3);
    void update(uint32_t enc0, uint32_t enc1, uint32_t enc2, uint32_t enc3);

    float get_output(uint8_t wheel) const;       // -100.0 ～ +100.0
    float get_wheel_omega(uint8_t wheel) const;  // [rad/s]
    float get_target_omega(uint8_t wheel) const; // [rad/s]
    float get_yaw_correction() const;            // [rad/s]

private:
    static constexpr float PI = 3.14159265358979323846f;
    static constexpr float SQRT2_INV = 0.7071067811865475f;

    float wheelRadius_;
    float rotationRadius_;
    float cpr_;
    float dt_;

    float vxCmd_ = 0.0f, vyCmd_ = 0.0f, wzCmd_ = 0.0f;
    float yawDeg_ = 0.0f, targetYawDeg_ = 0.0f;
    bool yawValid_ = false, yawHoldEnabled_ = true, yawTargetCaptured_ = false;
    float yawDeadband_ = 0.08f; // [rad/s]
    float yawKp_ = 2.0f, yawKi_ = 0.0f, yawKd_ = 0.0f;
    float yawIntegral_ = 0.0f, previousYawError_ = 0.0f;
    float yawCorrection_ = 0.0f;

//    int32_t encoderNow_[WHEEL_COUNT] = {};
    uint32_t encoderNow_[WHEEL_COUNT] = {};
//    int32_t encoderPrevious_[WHEEL_COUNT] = {};
    uint32_t encoderPrevious_[WHEEL_COUNT] = {};
    bool encoderInitialized_ = false;
    float wheelOmega_[WHEEL_COUNT] = {};
    float filteredOmega_[WHEEL_COUNT] = {};
    float targetOmega_[WHEEL_COUNT] = {};
    float output_[WHEEL_COUNT] = {};
    float kp_[WHEEL_COUNT] = {};
    float ki_[WHEEL_COUNT] = {};
    float kd_[WHEEL_COUNT] = {};
    float kS_[WHEEL_COUNT] = {};
    float kV_[WHEEL_COUNT] = {};
    float integral_[WHEEL_COUNT] = {};
    float previousMeasurement_[WHEEL_COUNT] = {};
    int8_t motorSign_[WHEEL_COUNT] = {1, 1, 1, 1};
    int8_t encoderSign_[WHEEL_COUNT] = {1, 1, 1, 1};

    static float clamp(float value, float minimum, float maximum);
    static float normalize_angle(float angleDeg);
    void update_yaw_control();
    void calculate_wheel_targets(float wz);
    void calculate_wheel_velocity();
    void calculate_wheel_pid();
    void reset_yaw_pid();
};

#endif	/* PID_HPP */

