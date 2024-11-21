#include <Arduino.h>
#include "inclination_sensor.h"
#include "speed_sensor.h"
#include "brake_controller.h"

#define HALL_SENSOR_PIN 4
#define SERVO_PIN 9

void setup() {
    Serial.begin(115200);

    // 각 모듈 초기화
    initInclinationSensor();
    initSpeedSensor(HALL_SENSOR_PIN);
    initBrakeController(SERVO_PIN);
}

void loop() {
    float currentSpeed = getSpeed();
    float currentInclination = getInclination();

    Serial.print("Speed: ");
    Serial.print(currentSpeed);
    Serial.print(" km/h, Inclination: ");
    Serial.print(currentInclination);
    Serial.println(" degrees");

    if (currentSpeed > 15.0 || currentInclination > 15.0) {
        activateBrake();
    }

    delay(100);
}
