#include "speed_sensor.h"
#include <Arduino.h>

volatile int pulseCount = 0;
unsigned long lastTime = 0;
const float wheelCircumference = 2.1; // 휠 둘레 (단위: m)

void IRAM_ATTR countPulse() {
  pulseCount++;
}

void initSpeedSensor(int pin) {
  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), countPulse, FALLING);
}

float getSpeed() {
  unsigned long currentTime = millis();
  float speed = 0;

  if (currentTime - lastTime >= 1000) { // 1초마다 계산
    float rpm = (pulseCount / 1.0) * 60; // 분당 회전수
    speed = (rpm * wheelCircumference * 60) / 1000; // 속도(km/h)
    pulseCount = 0; // 카운터 초기화
    lastTime = currentTime; // 시간 갱신
  }

  return speed;
}
