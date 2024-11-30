#include "speed_sensor.h"
#include <Arduino.h>

volatile int pulseCount = 0;
unsigned long lastPulseTime = 0;   // 마지막 펄스 시간
const float wheelCircumference = 0.9;  // 휠 둘레 (단위: m)
const int magnetCount = 2;  // 자석 개수

void IRAM_ATTR countPulse() {
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();

    // 50ms 이내의 중복 신호는 무시 (노이즈 필터링)
    if (interruptTime - lastInterruptTime > 50) {
        pulseCount++;
        Serial.print("Pulse detected: ");
        Serial.println(pulseCount);
    }
    lastInterruptTime = interruptTime;
}

void initSpeedSensor(int pin) {
    pinMode(pin, INPUT_PULLUP);  // 핀을 입력으로 설정하고 풀업 저항 활성화
    attachInterrupt(digitalPinToInterrupt(pin), countPulse, FALLING);  // FALLING 엣지에서 인터럽트 발생
}

float getSpeed() {
    unsigned long currentTime = millis();
    float speed = 0;

    if (pulseCount > 0) {
        // 펄스가 발생했을 경우 시간 차이를 기반으로 속도 계산
        unsigned long timeDiff = currentTime - lastPulseTime;  // 마지막 펄스 간 시간 차이(ms)
        float rpm = (60000.0 / timeDiff) * (pulseCount / (float)magnetCount);  // 분당 회전수 (RPM)

        speed = (rpm * wheelCircumference) * 3.6 / 40;  // 속도(km/h)

        pulseCount = 0;  // 펄스 카운트 초기화
        lastPulseTime = currentTime;  // 마지막 펄스 시간 갱신
    }

    return speed;
}
