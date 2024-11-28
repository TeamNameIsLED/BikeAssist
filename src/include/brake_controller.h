#ifndef BRAKE_CONTROLLER_H
#define BRAKE_CONTROLLER_H

void initBrakeController(int pin);   // 브레이크 초기화
void activateBrake(float speed, float inclination);     // 속도와 기울기에 따라 브레이크 작동

#endif
