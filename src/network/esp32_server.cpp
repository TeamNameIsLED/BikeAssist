#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Wi-Fi 네트워크 설정
#define WIFI_SSID "YOUR_WIFI_SSID"        // 주석: 자신의 Wi-Fi 네트워크 이름
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD" // 주석: 자신의 Wi-Fi 비밀번호

WebServer server(80); // 포트 80에서 웹 서버 실행
Adafruit_MPU6050 mpu;

// 웹 페이지 요청 처리 함수
void handleRoot() {
    server.send(200, "text/html", "<h1>Smart Bicycle Web Server</h1>");
}

// 센서 데이터를 JSON 형식으로 제공하는 함수
void handleData() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    float pitch = atan2(a.acceleration.v, a.acceleration.u) * 180 / PI; // 가상 변수 v, u 사용
    String json = "{\"pitch\": " + String(pitch, 2) + ", \"speed\": 5.00}";
    server.send(200, "application/json", json);
}

void setup() {
    Serial.begin(115200);

    // Wi-Fi 연결 설정
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // 웹 서버 경로 설정
    server.on("/", handleRoot);
    server.on("/data", handleData);

    // 서버 시작
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    // 웹 서버 클라이언트 요청 처리
    server.handleClient();
}
