#include <Arduino.h>
#include "inclination_sensor.h"
#include "speed_sensor.h"
#include "brake_controller.h"

#define HALL_SENSOR_PIN 4
#define SERVO_PIN 5

void setup() {
    Serial.begin(115200);  // 시리얼 통신 시작

    initInclinationSensor();   // 기울기 센서 초기화
    initSpeedSensor(HALL_SENSOR_PIN);  // 속도 센서 초기화
    initBrakeController(SERVO_PIN);  // 브레이크 제어 초기화
}

void loop() {
    float currentSpeed = getSpeed();  // 현재 속도 값
    float currentInclination = getInclination();  // 현재 기울기 값

    Serial.print("Speed: ");
    Serial.print(currentSpeed);
    Serial.print(" km/h, Inclination: ");
    Serial.print(currentInclination);
    Serial.println(" degrees");

    // 속도나 기울기가 임계값 초과 시 브레이크 작동
    if (currentSpeed > 15.0 || currentInclination > 15.0) {
        activateBrake(currentSpeed, currentInclination);  // 속도에 따라 브레이크 세기 조절
    } else {
        activateBrake(0, currentInclination);  // 속도가 낮으면 브레이크 해제
    }

    delay(100);  // 100ms 간격으로 데이터 확인
}
