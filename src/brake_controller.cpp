#include "brake_controller.h"
#include <ESP32Servo.h>

Servo brakeServo;  // 서보 모터 객체

// 브레이크 제어 초기화
void initBrakeController(int pin) {
    brakeServo.attach(pin);  // 서보 모터를 지정된 핀에 연결
}

// 브레이크 작동 (속도에 따라 제어)
void activateBrake(float speed) {
    int maxAngle = 270;   // 서보 모터의 최대 각도 (최대 브레이크)
    int minAngle = 0;    // 서보 모터의 최소 각도 (브레이크 해제)
    
    int brakeAngle = 0;

    // 속도가 15 이상일 때 브레이크 작동
    if (speed > 15.0) {
        // 속도에 따라 서보 모터 각도 조정
        brakeAngle = map(speed, 15, 30, minAngle, maxAngle);  // 15km/h에서 30km/h까지 각도 증가
    } else {
        // 속도가 15 이하일 때 브레이크 해제 (각도를 0도로 설정)
        brakeAngle = minAngle;  // 서보 모터를 0도로 설정하여 브레이크 해제
    }

    brakeServo.write(brakeAngle);  // 서보 모터를 설정된 각도로 회전
    Serial.print("Applying brake at angle: ");
    Serial.println(brakeAngle);
}
