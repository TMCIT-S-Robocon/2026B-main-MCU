///* 
// * File:   pid.hpp
// * Author: natsu217
// *
// * Created on August 24, 2026, 12:29 AM
// */
//
//#ifndef PID_HPP_
//#define	PID_HPP_
//
//#include <stdint.h>
//#include <stdbool.h>
//
//class Omni4PID{
//public:
//    struct WheelSpeed{
//        float w0;
//        float w1;
//        float w2;
//        float w3;
//    };
//    struct MotorCommand{
//        float m0;
//        float m1;
//        float m2;
//        float m3;
//    };
//    
//    Omni4PID();
//    // 初期化
//    void init();
//    // tmr(5ms)周期処理
//    void update();
//    // 目標速度[m/s], [rad/s]
//    void setVelocity(float vx, float vy, float wz);
//    // 目標位置[m], [deg]
//    void setTargetPosition(float x, float y);
//    void setTargetyaw(float yaw);
//    // 姿勢制御を使用するか
//    void setAttitudeControl(bool enable);
//    // 位置制御を使用するか
//    void setPositionControl(bool enable);
//    // 現在のyawを設定
//    void setYaw(float yaw);
//    // getter
//    float getX() const;
//    float getY() const;
//    float getYaw() const;
//    float getVxBody() const;
//    float getVyBody() const;
//    WheelSpeed getWheelSpeed() const;
//    WheelSpeed getTargetWheelSpeed() const;
//    MotorCommand getMotorCommand() const;
//    // 個別ゲイン設定
//    void setVelocityPID(float kp, float ki, float kd);
//    void setYawPID(float kp, float ki, float kd);
//    void setPositionGain(float kp, float kd);
//    // タイマー割り込みから呼ぶ関数
//    static void tmr2_isr();
//    
//private:
//    static Omni4PID* instance;
//    
//    static constexpr float DT = 0.005f;
//    static constexpr float PI = 3.14159265358979323846f;
//    // エンコーダー
//    static constexpr float PPR = 2048.0f;
//    static constexpr float CPR = PPR * 4.0f;
//    static constexpr float WHEEL_RADIUS = 0.05f;
//    // 機体の中心から車輪まで
//    static constexpr float L = 0.0f; // あとでかえる
//    
//    // エンコーダー
//    uint32_t enc0;
//    uint32_t enc1;
//    uint32_t enc2;
//    uint32_t enc3;
//    uint32_t enc0_prev;
//    uint32_t enc1_prev;
//    uint32_t enc2_prev;
//    uint32_t enc3_prev;
//    // wheel velocity
//    WheelSpeed wheelSpeed;
//    WheelSpeed targetWheelSpeed;
//    WheelSpeed wheelSpeedFiltered;
//    // motor command
//    MotorCommand motorCommand;
//    // 速度PID
//    float velocityKp;
//    float velocityKi;
//    float velocityKd;
//    float integral[4];
//    float prevMeasurement[4];
//    // ロボットの速度
//    float vx;
//    float vy;
//    float wz;
//    float vxBody;
//    float vyBody;
//    // 位置
//    float x;
//    float y;
//    float targetX;
//    float targetY;
//    // attitude
//    float yaw;
//    float targetYaw;
//    float yawIntegral;
//    float prevYawError;
//    float yawKp;
//    float yawKi;
//    float yawKd;
//    bool attitudeEnabled;
//    bool positionEnabled;
//    // 位置PID
//    float positionKp;
//    float positionKd;
//    // 関数
//    void readEncoder();
//    void updateWheelSpeed();
//    void updateOdometry();
//    void updatePositionControl();
//    void updateAttitudeControl();
//    void inverseKinematics();
//    void updateVelocityPID();
//    float normalizeAngle(float angle);
//    float clamp(float value, float minValue, float maxValue);
//    void resetPID();
//};
//
//#endif	/* PID_HPP */
//
