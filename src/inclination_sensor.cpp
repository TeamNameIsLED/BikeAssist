#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void initInclinationSensor() {
    if (!mpu.begin()) {
        Serial.println("Failed to initialize MPU6050!");
        while (1);  // 초기화 실패 시 무한 대기
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);  // 가속도계 범위 설정
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);       // 자이로 범위 설정
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);    // 필터 설정
}

float getInclination() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);  // 센서 데이터 읽기

    // 기울기 계산 (pitch angle)
    float pitch = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;
    return pitch;  // 기울기 값을 반환
}
