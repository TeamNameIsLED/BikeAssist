#include <Arduino.h>
#include "inclination_sensor.h"
#include "speed_sensor.h"
#include "brake_controller.h"

#define HALL_SENSOR_PIN 4
#define SERVO_PIN 9

void setup() {
    Serial.begin(115200);  // 시리얼 통신 시작

    // 각 모듈 초기화
    initInclinationSensor();   // 기울기 센서 초기화
    initSpeedSensor(HALL_SENSOR_PIN);  // 속도 센서 초기화
    initBrakeController(SERVO_PIN);  // 브레이크 제어 초기화
}

void loop() {
    float currentSpeed = getSpeed();  // 속도 측정 함수
    float currentInclination = getInclination();  // 기울기 측정 함수

    // 현재 속도와 기울기 출력
    Serial.print("Speed: ");
    Serial.print(currentSpeed);
    Serial.print(" km/h, Inclination: ");
    Serial.print(currentInclination);
    Serial.println(" degrees");

    // 속도나 기울기가 15을 초과하면 브레이크 작동
    if (currentSpeed > 15.0 || currentInclination > 15.0) {
        activateBrake();  // 브레이크 작동
    }

    delay(100);  // 루프 간 딜레이
}
