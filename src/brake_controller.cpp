#include "brake_controller.h"
#include <ESP32Servo.h>

Servo brakeServo;  // 서보 모터 객체

// 브레이크 제어 초기화
void initBrakeController(int pin) {
    brakeServo.attach(pin);  // 서보 모터를 지정된 핀에 연결
}

// 브레이크 작동 (속도에 따라 제어)
void activateBrake(float speed) {
    int maxAngle = 270;   // 서보 모터의 최대 각도 (최대 브레이크 힘)
    int minAngle = 0;    // 서보 모터의 최소 각도 (브레이크 해제)
    float maxSpeed = 30.0;  // 최대 속도 (30km/h에서 최대 브레이크)
    
    // 속도에 비례하여 서보 모터 각도 계산
    int brakeAngle = map(speed, 0, maxSpeed, minAngle, maxAngle);
    brakeAngle = constrain(brakeAngle, minAngle, maxAngle);  // 각도 제한

    brakeServo.write(brakeAngle);  // 서보 모터를 설정된 각도로 회전
    Serial.print("Applying brake at angle: ");
    Serial.println(brakeAngle);
}
