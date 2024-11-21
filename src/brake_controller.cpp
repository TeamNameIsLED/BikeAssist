#include <ESP32Servo.h>  // ESP32용 서보 모터 라이브러리

Servo brakeServo;  // 서보 모터 객체

void initBrakeController(int pin) {
    brakeServo.attach(pin);  // 서보 모터를 지정된 핀에 연결
}

void activateBrake() {
    brakeServo.write(90);  // 서보 모터를 90도로 회전시켜 브레이크 작동
    delay(2000);           // 2초 동안 브레이크 유지
    brakeServo.write(0);   // 서보 모터를 0도로 회전시켜 브레이크 해제
}
