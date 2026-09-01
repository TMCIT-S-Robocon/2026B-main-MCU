//#include "pid.hpp"
//#include <math.h>
//
//Omni4PID::Omni4PID(
//    float wheel_radius_,
//    float robot_radius_,
//    uint32_t cpr_
//)
//{
//    wheel_radius = wheel_radius_;
//    robot_radius = robot_radius_;
//    cpr = cpr_;
//    
//    vx_cmd = 0.0f;
//    vy_cmd = 0.0f;
//    wz_cmd = 0.0f;
//    current_yaw = 0.0f;
//    target_yaw = 0.0f;
//    yaw_hold = false;
//    yaw_kp = 0.5f;
//    yaw_ki = 0.0f;
//    yaw_kd = 0.0f;
//    yaw_integral = 0.0f;
//    yaw_prev_error = 0.0f;
//    max_wz = 1.5f;
//    
//    for(int i = 0; i < WHEEL_COUNT; i++){
//        encoder_now[i] = 0;
//        encoder_prev[i] = 0;
//        encoder_prev[i] = 0;
//        wheel_omega[i] = 0.0f;
//        wheel_omega_filtered[i] = 0.0f;
//        target_omega[i] = 0.0f;
//        
//        // ゲイン
//        kp[i] = 0.0f;
//        ki[i] = 0.0f;
//        kd[i] = 0.0f;
//        
//        integral[i] = 0.0f;
//        prev_error[i] = 0.0f;
//        output[i] = 0.0f;
//        motor_sign[i] = 1;
//    }
//}
//
//void Omni4PID::set_encoder_count(uint8_t wheel, int32_t count){
//    if(wheel >= WHEEL_COUNT) return;
//    
//    encoder_now[wheel] = count;
//}
//
//void Omni4PID::set_encoder_counts(int32_t e0, int32_t e1, int32_t e2, int32_t e3){
//    encoder_now[0] = e0;
//    encoder_now[1] = e1;
//    encoder_now[2] = e2;
//    encoder_now[3] = e3;
//}
//
//void Omni4PID::set_velocity(float vx, float vy, float wz){
//    vx_cmd = vx;
//    vy_cmd = vy;
//    wz_cmd = wz;
//}
//
//void Omni4PID::set_yaw(float yaw_deg){
//    current_yaw = normalize_angle(yaw_deg);
//}
//
//void Omni4PID::set_target_yaw(float yaw_deg){
//    target_yaw = normalize_angle(yaw_deg);
//}
//
//void Omni4PID::enable_yaw_hold(bool enable){
//    yaw_hold = enable;
//    
//    if(!enable){
//        yaw_integral = 0.0f;
//        yaw_prev_error = 0.0f;
//    }
//}
//
//void Omni4PID::set_wheel_pid(uint8_t wheel, float kp_, float ki_, float kd_){
//    if(wheel >= WHEEL_COUNT) return;
//    
//    kp[wheel] = kp_;
//    ki[wheel] = ki_;
//    kd[wheel] = kd_;
//}
//
//void Omni4PID::set_all_wheel_pid(float kp_, float ki_, float kd_){
//    for(int i = 0; i < WHEEL_COUNT; i++){
//        kp[i] = kp_;
//        ki[i] = ki_;
//        kd[i] = kd_;    
//    }
//}
//    
//void Omni4PID::set_yaw_pid(float kp_, float ki_, float kd_){
//    yaw_kp = kp_;
//    yaw_ki = ki_;
//    yaw_kd = kd_;
//}
//
//void Omni4PID::set_motor_sign(uint8_t wheel, int8_t sign){
//    if(wheel >= WHEEL_COUNT) return;
//    
//    motor_sign[wheel] = (sign >= 0) ? 1: -1;
//}
//
//float Omni4PID::normalize_angle(float angle){
//    while(angle > 180.0f){
//        angle -= 360.0f;
//    }
//    while(angle < -180.0f){
//        angle += 360.0f;
//    }
//    
//    return angle;
//}
//
//// 4輪オムニ逆運動学
//// chassis.hppと合わせる
//// * vx : +x
//// * vy : +y
//// * wz : CCW
//// *
//// *
//// * 車輪角速度[rad/s]
//// *
//// * w0 = (-(vx + vy)/sqrt(2) + L*wz) / R
//// * w1 = ( +(vx - vy)/sqrt(2) + L*wz) / R
//// * w2 = ( +(vx + vy)/sqrt(2) + L*wz) / R
//// * w3 = ( +(vy - vx)/sqrt(2) + L*wz) / R
//// モーターの実際の回転方向が逆ならset_motor_sign()で調整する
//
//void Omni4PID::calculate_wheel_targets(){
//    constexpr float SQRT2_INV = 0.70710678118f;
//    
//    target_omega[0] = (-SQRT2_INV*vx_cmd - SQRT2_INV*vy_cmd + robot_radius*wz_cmd) / wheel_radius;
//    target_omega[1] = (+SQRT2_INV*vx_cmd - SQRT2_INV*vy_cmd + robot_radius*wz_cmd) / wheel_radius;
//    target_omega[2] = (+SQRT2_INV*vx_cmd + SQRT2_INV*vy_cmd + robot_radius*wz_cmd) / wheel_radius;
//    target_omega[3] = (-SQRT2_INV*vx_cmd + SQRT2_INV*vy_cmd + robot_radius*wz_cmd) / wheel_radius;
//}
//
//// エンコーダー速度計算
//// 車輪側についてるので
//// omega = delta_count * 2pi / CPR / dt
//void Omni4PID::calculate_wheel_velocity(){
//    constexpr float ALPHA = 0.25f;
//    
//    for(int i = 0; i < WHEEL_COUNT; i++){
//        // 10ms分のカウント差
//        int32_t delta = encoder_now[i] - encoder_prev2[i];
//        // 車輪角速度
//        wheel_omega[i] = ((float)delta * 2.0f * (float)M_PI / (float)cpr * VELOCITY_PERIOD);
//        // ローパスフィルタ
//        wheel_omega_filtered[i] = ALPHA * wheel_omega[i] + (1.0f - ALPHA) * wheel_omega_filtered[i];
//        
//        wheel_omega[i] = wheel_omega_filtered[i];
//    }
//    
//    // 値の更新
//    for(int i = 0; i < WHEEL_COUNT; i++){
//        encoder_prev2[i] = encoder_now[i];
//    }
//}
//
//// ヨー角PID
//void Omni4PID::calculate_yaw_control(){
//    if(!yaw_hold) return;
//    
//    float error_deg = normalize_angle(target_yaw - current_yaw);
//    float error_rad = error_deg * (float)M_PI / 180.0f;
//    
//    // 停止時などに積分が残らないようにする
//    if(fabsf(error_rad) < 0.5f){
//        yaw_integral *= 0.95f;
//    }
//    
//    yaw_integral += error_rad * CONTROL_PERIOD;
//    
//    // 積分制限
//    constexpr float YAW_INTEGRAL_LIMIT = 1.0f;
//    if(yaw_integral > YAW_INTEGRAL_LIMIT){
//        yaw_integral = YAW_INTEGRAL_LIMIT;
//    }
//    if(yaw_integral < -YAW_INTEGRAL_LIMIT){
//        yaw_integral = -YAW_INTEGRAL_LIMIT;
//    }
//    
//    float derivative = (error_rad - yaw_prev_error) / CONTROL_PERIOD;
//    float correction = yaw_kp * error_rad * yaw_ki * yaw_integral + yaw_kd * derivative;
//    
//    if(correction > max_wz){
//        correction = max_wz;
//    }
//    if(correction < -max_wz){
//        correction = -max_wz;
//    }
//    
//    // ここで元々の各速度に補正を加える
//    wz_cmd += correction;
//    yaw_prev_error = error_rad;
//}
//
//// 速度PID
//void Omni4PID::calculate_wheel_pid(){
//    for(int i = 0; i < WHEEL_COUNT; i++){
//        float error = target_omega[i] - wheel_omega[i];
//        
//        // ほぼ停止状態
//        if(fabsf(target_omega[i]) < 0.05f && fabsf(wheel_omega[i]) < 0.05f){
//            integral[i] = 0.0f;
//            prev_error[i] = 0.0f;
//            output[i] = 0.0f;
//            continue;
//        }
//        
//        // 積分
//        integral[i] += error * CONTROL_PERIOD;
//        // 積分のワインドアップ防止
//        constexpr float INTEGRAL_LIMIT = 5.0f;
//        if(integral[i] > INTEGRAL_LIMIT){
//            integral[i] = INTEGRAL_LIMIT;
//        }
//        if(integral[i] < -INTEGRAL_LIMIT){
//            integral[i] = -INTEGRAL_LIMIT;
//        }
//        
//        // 微分
//        float derivative = (error - prev_error[i]) / CONTROL_PERIOD;
//        
//        // PID
//        float u = kp[i] * error + ki[i] * integral[i] + kd[i] * derivative;
//        
//        // 出力制限
//        constexpr float OUTPUT_LIMIT = 100.0f;
//        if(u > OUTPUT_LIMIT){
//            u = OUTPUT_LIMIT;
//            // アンチワインドアップ
//            if(error > 0.0f){
//                integral[i] -= error * CONTROL_PERIOD;
//            }
//        } else if(u < -OUTPUT_LIMIT){
//            u = -OUTPUT_LIMIT;
//            // アンチワインドアップ
//            if(error < 0.0f){
//                integral[i] -= error * CONTROL_PERIOD;
//            }
//        }
//        
//        output[i] = u * (float)motor_sign[i];
//        
//        prev_error[i] = error;
//    }
//}
//
//// メイン制御
//void Omni4PID::update(){
//    // 姿勢制御
//    if(yaw_hold){
//        calculate_yaw_control();
//    }
//    // 速度指令 -> 各車輪の目標速度を出す
//    calculate_wheel_targets();
//    // エンコーダー -> 実速度
//    calculate_wheel_velocity();
//    // 速度PID
//    calculate_wheel_pid();
//}
//
//float Omni4PID::get_output(uint8_t wheel) const{
//    if(wheel >= WHEEL_COUNT){
//        return 0.0f;
//    }
//    
//    return output[wheel];
//}
//
//
//int Omni4PID::get_output_int(uint8_t wheel) const{
//    if(wheel >= WHEEL_COUNT){
//        return 0;
//    }
//
//    float value = output[wheel];
//    if(value > 100.0f){
//        value = 100.0f;
//    }
//    if(value < -100.0f){
//        value = -100.0f;
//    }
//
//    return (int)value;
//}
//
//float Omni4PID::get_wheel_omega(uint8_t wheel) const{
//    if(wheel >= WHEEL_COUNT){
//        return 0.0f;
//    }
//
//    return wheel_omega[wheel];
//}
//
//float Omni4PID::get_target_omega(uint8_t wheel) const{
//    if(wheel >= WHEEL_COUNT){
//        return 0.0f;
//    }
//
//    return target_omega[wheel];
//}
//
//float Omni4PID::get_vx() const{
//    return vx_cmd;
//}
//
//float Omni4PID::get_vy() const{
//    return vy_cmd;
//}
//
//float Omni4PID::get_wz() const{
//    return wz_cmd;
//}