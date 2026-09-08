#include "pid.hpp"
#include <math.h>

Omni4PID::Omni4PID(float wheelRadius, float rotationRadius, float cpr,
                   float controlPeriod)
    : wheelRadius_(wheelRadius), rotationRadius_(rotationRadius),
      cpr_(cpr), dt_(controlPeriod) {}

void Omni4PID::set_velocity(float vx, float vy, float wz) {
    vxCmd_ = vx;
    vyCmd_ = vy;
    wzCmd_ = wz;
}

void Omni4PID::set_yaw(float yawDeg, bool valid) {
    yawDeg_ = normalize_angle(yawDeg);
    yawValid_ = valid;
}

void Omni4PID::set_target_yaw(float yawDeg) {
    targetYawDeg_ = normalize_angle(yawDeg);
    yawTargetCaptured_ = true;
    reset_yaw_pid();
}

void Omni4PID::set_yaw_hold_enabled(bool enabled) {
    yawHoldEnabled_ = enabled;
    yawTargetCaptured_ = false;
    reset_yaw_pid();
}

void Omni4PID::set_yaw_hold_deadband(float wzDeadband) {
    yawDeadband_ = (wzDeadband >= 0.0f) ? wzDeadband : -wzDeadband;
}

void Omni4PID::set_wheel_pid(uint8_t wheel, float kp, float ki, float kd) {
    if (wheel >= WHEEL_COUNT) return;
    kp_[wheel] = kp;
    ki_[wheel] = ki;
    kd_[wheel] = kd;
}

void Omni4PID::set_all_wheel_pid(float kp, float ki, float kd) {
    for (uint8_t i = 0; i < WHEEL_COUNT; ++i) set_wheel_pid(i, kp, ki, kd);
}

void Omni4PID::set_yaw_pid(float kp, float ki, float kd) {
    yawKp_ = kp;
    yawKi_ = ki;
    yawKd_ = kd;
    reset_yaw_pid();
}

void Omni4PID::set_motor_sign(uint8_t wheel, int8_t sign) {
    if (wheel < WHEEL_COUNT) motorSign_[wheel] = (sign < 0) ? -1 : 1;
}

void Omni4PID::set_encoder_sign(uint8_t wheel, int8_t sign) {
    if (wheel < WHEEL_COUNT) encoderSign_[wheel] = (sign < 0) ? -1 : 1;
}

void Omni4PID::update(int32_t enc0, int32_t enc1, int32_t enc2, int32_t enc3) {
    encoderNow_[0] = enc0; encoderNow_[1] = enc1;
    encoderNow_[2] = enc2; encoderNow_[3] = enc3;

    if (!encoderInitialized_) {
        for (uint8_t i = 0; i < WHEEL_COUNT; ++i) encoderPrevious_[i] = encoderNow_[i];
        encoderInitialized_ = true;
        return; // 初回は速度推定しない
    }

    update_yaw_control();
    calculate_wheel_targets(wzCmd_ + yawCorrection_);
    calculate_wheel_velocity();
    calculate_wheel_pid();
}

void Omni4PID::update_yaw_control() {
    yawCorrection_ = 0.0f;
    if (!yawHoldEnabled_ || !yawValid_) return;

    // 回転を操縦している間は追従せず、スティックを戻した瞬間の角度を保持する。
    if (fabsf(wzCmd_) > yawDeadband_) {
        targetYawDeg_ = yawDeg_;
        yawTargetCaptured_ = true;
        reset_yaw_pid();
        return;
    }
    if (!yawTargetCaptured_) {
        targetYawDeg_ = yawDeg_;
        yawTargetCaptured_ = true;
        reset_yaw_pid();
        return;
    }

    const float errorRad = normalize_angle(targetYawDeg_ - yawDeg_) * PI / 180.0f;
    const float derivative = (errorRad - previousYawError_) / dt_;
    yawIntegral_ = clamp(yawIntegral_ + errorRad * dt_, -0.6f, 0.6f);
    yawCorrection_ = clamp(yawKp_ * errorRad + yawKi_ * yawIntegral_ + yawKd_ * derivative,
                           -1.5f, 1.5f);
    previousYawError_ = errorRad;
}

void Omni4PID::calculate_wheel_targets(float wz) {
    targetOmega_[0] = (-SQRT2_INV * vxCmd_ - SQRT2_INV * vyCmd_ + rotationRadius_ * wz) / wheelRadius_;
    targetOmega_[1] = ( SQRT2_INV * vxCmd_ - SQRT2_INV * vyCmd_ + rotationRadius_ * wz) / wheelRadius_;
    targetOmega_[2] = ( SQRT2_INV * vxCmd_ + SQRT2_INV * vyCmd_ + rotationRadius_ * wz) / wheelRadius_;
    targetOmega_[3] = (-SQRT2_INV * vxCmd_ + SQRT2_INV * vyCmd_ + rotationRadius_ * wz) / wheelRadius_;
}

void Omni4PID::calculate_wheel_velocity() {
    constexpr float FILTER_ALPHA = 0.30f;
    for (uint8_t i = 0; i < WHEEL_COUNT; ++i) {
        const int32_t deltaCount = encoderNow_[i] - encoderPrevious_[i];
        const float rawOmega = (float)(encoderSign_[i] * deltaCount) * 2.0f * PI / cpr_ / dt_;
        filteredOmega_[i] += FILTER_ALPHA * (rawOmega - filteredOmega_[i]);
        wheelOmega_[i] = filteredOmega_[i];
        encoderPrevious_[i] = encoderNow_[i];
    }
}

void Omni4PID::calculate_wheel_pid() {
    constexpr float OUTPUT_LIMIT = 100.0f;
    constexpr float INTEGRAL_LIMIT = 20.0f;
    for (uint8_t i = 0; i < WHEEL_COUNT; ++i) {
        const float error = targetOmega_[i] - wheelOmega_[i];
        const float measurementDerivative = (wheelOmega_[i] - previousMeasurement_[i]) / dt_;
        const float candidateIntegral = clamp(integral_[i] + error * dt_, -INTEGRAL_LIMIT, INTEGRAL_LIMIT);
        const float unsaturated = kp_[i] * error + ki_[i] * candidateIntegral - kd_[i] * measurementDerivative;
        const float command = clamp(unsaturated, -OUTPUT_LIMIT, OUTPUT_LIMIT);

        // 飽和をさらに強める向きには積分しない。
        if (command == unsaturated || (command >= OUTPUT_LIMIT && error < 0.0f) ||
            (command <= -OUTPUT_LIMIT && error > 0.0f)) {
            integral_[i] = candidateIntegral;
        }
        output_[i] = command * (float)motorSign_[i];
        previousMeasurement_[i] = wheelOmega_[i];
    }
}

float Omni4PID::get_output(uint8_t wheel) const {
    return (wheel < WHEEL_COUNT) ? output_[wheel] : 0.0f;
}
float Omni4PID::get_wheel_omega(uint8_t wheel) const {
    return (wheel < WHEEL_COUNT) ? wheelOmega_[wheel] : 0.0f;
}
float Omni4PID::get_target_omega(uint8_t wheel) const {
    return (wheel < WHEEL_COUNT) ? targetOmega_[wheel] : 0.0f;
}
float Omni4PID::get_yaw_correction() const { return yawCorrection_; }

float Omni4PID::clamp(float value, float minimum, float maximum) {
    return (value < minimum) ? minimum : ((value > maximum) ? maximum : value);
}
float Omni4PID::normalize_angle(float angleDeg) {
    while (angleDeg > 180.0f) angleDeg -= 360.0f;
    while (angleDeg <= -180.0f) angleDeg += 360.0f;
    return angleDeg;
}
void Omni4PID::reset_yaw_pid() {
    yawIntegral_ = 0.0f;
    previousYawError_ = 0.0f;
    yawCorrection_ = 0.0f;
}
