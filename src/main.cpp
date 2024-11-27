#include <Arduino.h>
#include <AWS_IOT.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

#define LED_PIN 16 // LED 핀 번호

AWS_IOT awsIot;
const char* ssid = "MJS-Guest";
const char* password = "00908764801015";
char HOST_ADDRESS[] = "a3pecuomf1y0cd-ats.iot.ap-northeast-2.amazonaws.com";
char CLIENT_ID[] = "ESP32_BIKEASSIST";
char SHADOW_UPDATE_TOPIC[] = "$aws/things/ESP32_BIKEASSIST/shadow/update"; // Shadow 업데이트 주제
char SHADOW_GET_ACCEPTED_TOPIC[] = "$aws/things/ESP32_BIKEASSIST/shadow/update/accepted"; // Shadow 상태 업데이트 확인 주제

int msgReceived = 0;
char payload[512];
char rcvdPayload[512];

void mySubCallBackHandler(char *topicName, int payloadLen, char *payLoad) {
    strncpy(rcvdPayload, payLoad, payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}

void setup() {
    Serial.begin(115200); // 시리얼 통신 시작
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

        // Shadow 상태 업데이트 확인을 위한 구독 설정
        if(awsIot.subscribe(SHADOW_GET_ACCEPTED_TOPIC, mySubCallBackHandler) == 0) {
            Serial.println("Subscribe to Shadow accepted topic Successful");
        } else {
            Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
            while (1);
        }
    } else {
        Serial.println("AWS connection failed, check the HOST Address and credentials");
        while (1);
    }

    Serial.println("Please enter the speed value:"); // 속도 값을 입력하라는 안내 메시지
}

void loop() {
    if (Serial.available() > 0) { // 시리얼로부터 입력된 데이터가 있을 경우
        String input = Serial.readStringUntil('\n'); // 입력된 문자열을 읽음
        float speed = input.toFloat(); // 입력된 문자열을 float 타입으로 변환

        Serial.print("Entered Speed: ");
        Serial.println(speed);

        // Shadow에 속도 정보 업데이트
        snprintf(payload, sizeof(payload), "{\"state\": {\"reported\": {\"speed\": %.2f}}}", speed);
        if (awsIot.publish(SHADOW_UPDATE_TOPIC, payload) == 0) {
            Serial.print("Published Speed to AWS Shadow: ");
            Serial.println(payload);
        } else {
            Serial.println("Publish to AWS Shadow failed");
        }
    }

    // Shadow의 desired 상태에 따른 LED 제어
    if (msgReceived == 1) {
        msgReceived = 0;

        // 수신된 Shadow 상태 정보 파싱
        JSONVar parsedPayload = JSON.parse(rcvdPayload);
        if (JSON.typeof(parsedPayload) == "undefined") {
            Serial.println("Parsing failed for received payload!");
            return;
        }

        // desired 상태에서 led 정보 추출
        JSONVar desiredState = parsedPayload["state"]["desired"];
        if (desiredState.hasOwnProperty("led")) {
            String ledState = (const char*) desiredState["led"];

            Serial.print("LED will be ");
            Serial.println(ledState);
            if (ledState == "ON") {
                digitalWrite(LED_PIN, HIGH); // LED 켬
            } else if (ledState == "OFF") {
                digitalWrite(LED_PIN, LOW); // LED 끔
            }
        }
    }

    delay(100); // 루프 간 딜레이
}
