#include "brake_controller.h"
#include <ESP32Servo.h>

Servo brakeServo;  // 서보 모터 객체

#define LED_PIN 16  // LED가 연결된 핀

// 브레이크 제어 초기화
void initBrakeController(int pin) {
    brakeServo.attach(pin);  // 서보 모터를 지정된 핀에 연결
    pinMode(LED_PIN, OUTPUT);  // LED 핀을 출력으로 설정
    digitalWrite(LED_PIN, LOW);  // 초기 상태에서 LED 끄기
}

// 브레이크 작동 (속도와 기울기에 따라 제어)
void activateBrake(float speed, float inclination) {
    int maxAngle = 270;   // 서보 모터의 최대 각도 (최대 브레이크)
    int minAngle = 0;     // 서보 모터의 최소 각도 (브레이크 해제)

    int brakeAngle = 0;   // 초기 브레이크 각도

    // 속도에 따른 브레이크 각도 설정
    if (speed > 15.0) {
        brakeAngle = map(speed, 15, 30, minAngle, maxAngle);  // 속도에 따라 각도 증가
    }

    // 기울기에 따른 브레이크 각도 설정
    if (inclination > 15.0) {
        brakeAngle = map(inclination, 15, 30, minAngle, maxAngle);  // 기울기에 따라 각도 증가
    }
    brakeAngle = constrain(brakeAngle, minAngle, maxAngle);  // 각도 제한

    brakeServo.write(brakeAngle);  // 서보 모터를 설정된 각도로 회전

    // 서보모터 작동 시 LED 켜기
    if (brakeAngle > minAngle) {
        digitalWrite(LED_PIN, HIGH);  // LED 켜기
        Serial.print("LED ON ");
    } else {
        digitalWrite(LED_PIN, LOW);   // LED 끄기
        Serial.print("LED OFF ");
    }

    // 디버깅 출력
    Serial.print("Applying brake at angle: ");
    Serial.println(brakeAngle);
}
