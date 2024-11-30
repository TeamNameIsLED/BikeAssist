#include <Arduino.h>
#include "inclination_sensor.h"
#include "speed_sensor.h"
#include "brake_controller.h"
#include <AWS_IOT.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

#define HALL_SENSOR_PIN 4
#define SERVO_PIN 5
#define LED_PIN 16 // LED 핀 번호

AWS_IOT awsIot;
const char* ssid = "MJS-Guest";
const char* password = "00908764801015";
char HOST_ADDRESS[] = "a3pecuomf1y0cd-ats.iot.ap-northeast-2.amazonaws.com";
char CLIENT_ID[] = "SpeedMonitorDevice";
char SHADOW_UPDATE_TOPIC[] = "$aws/things/ESP32_BIKEASSIST/shadow/update"; // Shadow 업데이트 주제

int state = 0; // 상태 변수: 0 = 속도 낮음, 1 = 속도 높음(메시지 전송됨)
extern volatile int pulseCount;

void setup() {
    Serial.begin(115200);  // 시리얼 통신 시작
    pinMode(LED_PIN, OUTPUT); // LED 핀을 출력 모드로 설정

    // WiFi 연결
    WiFi.disconnect(true);
    delay(1000);
    WiFi.mode(WIFI_STA);
    delay(1000);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to WiFi");

    // AWS IoT 연결
    if(awsIot.connect(HOST_ADDRESS, CLIENT_ID) == 0) {
        Serial.println("Connected to AWS IoT");
        delay(1000);
    } else {
        Serial.println("AWS connection failed, check the HOST Address and credentials");
        while (1);
    }

    initInclinationSensor();   // 기울기 센서 초기화
    initSpeedSensor(HALL_SENSOR_PIN);  // 속도 센서 초기화
    initBrakeController(SERVO_PIN);  // 브레이크 제어 초기화
}

void loop() {
    float currentSpeed = getSpeed();  // 현재 속도 값
    float currentInclination = getInclination();  // 현재 기울기 값

    Serial.print("Pulse Count: ");
    Serial.print(pulseCount);
    Serial.print(", Speed: ");
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

    // AWS Shadow에 속도 정보를 업데이트
    char payload[512];
    snprintf(payload, sizeof(payload), "{\"state\": {\"reported\": {\"speed\": %.2f}}}", currentSpeed);
    if (awsIot.publish(SHADOW_UPDATE_TOPIC, payload) == 0) {
        Serial.print("Published Speed to AWS Shadow: ");
        Serial.println(payload);
    } else {
        Serial.println("Publish to AWS Shadow failed");
    }

    // 속도가 30을 처음 초과하는 경우 문자 전송 및 LED 켜기
    if (currentSpeed > 30.0 && state == 0) {
        state = 1; // 상태를 1로 설정하여 이미 문자 보냈음을 표시
        digitalWrite(LED_PIN, HIGH); // LED 켜기
        Serial.println("LED is ON due to high speed");

        // Shadow에 상태 업데이트 (속도가 높아졌음을 표시)
        snprintf(payload, sizeof(payload), "{\"state\": {\"reported\": {\"speed\": %.2f, \"status\": \"HIGH\"}}}", currentSpeed);
        if (awsIot.publish(SHADOW_UPDATE_TOPIC, payload) == 0) {
            Serial.print("Published Speed and Status to AWS Shadow: ");
            Serial.println(payload);
        } else {
            Serial.println("Publish to AWS Shadow failed");
        }
    }

    // 속도가 30 이하로 떨어지는 경우 LED 끄고 상태 초기화
    if (currentSpeed <= 30.0 && state == 1) {
        state = 0; // 상태를 0으로 리셋
        digitalWrite(LED_PIN, LOW); // LED 끄기
        Serial.println("LED is OFF");

        // Shadow에 상태 업데이트 (속도가 낮아졌음을 표시)
        snprintf(payload, sizeof(payload), "{\"state\": {\"reported\": {\"speed\": %.2f, \"status\": \"LOW\"}}}", currentSpeed);
        if (awsIot.publish(SHADOW_UPDATE_TOPIC, payload) == 0) {
            Serial.print("Published Speed and Status to AWS Shadow: ");
            Serial.println(payload);
        } else {
            Serial.println("Publish to AWS Shadow failed");
        }
    }

    delay(500); // 500ms 간격으로 데이터 확인
}
