#include "brake_controller.h"
#include <Servo.h>

Servo brakeServo;

void initBrakeController(int pin) {
  brakeServo.attach(pin); // MG995 서보 모터 핀 설정
}

void activateBrake() {
  brakeServo.write(90); // 브레이크 작동 (90도로 당김)
  delay(2000);          // 2초 동안 브레이크 유지
  brakeServo.write(0);  // 브레이크 해제 (0도로 복귀)
}
