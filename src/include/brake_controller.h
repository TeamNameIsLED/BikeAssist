#ifndef BRAKE_CONTROLLER_H
#define BRAKE_CONTROLLER_H

#include <Arduino.h>
#include <ESP32Servo.h>

// 서보 모터 객체
extern Servo brakeServo;

// LED 핀 정의
#define LED_PIN 16 // LED가 연결된 핀

// 서보 모터 및 LED 초기화 함수
void initBrakeController(int pin);

// 브레이크 작동 함수
void activateBrake(float speed, float inclination);

#endif
