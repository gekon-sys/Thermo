#pragma once

#include "Motor.h"
#include <ESP32Servo.h>

class MyServo : public Motor
{
private:
    uint8_t pin;
    bool state;
    uint8_t hz;
    int minPulse;
    int maxPulse;
    Servo servo;

public:
    MyServo(uint8_t pin, uint8_t hz, int minPulse, int maxPulse, bool state = false);
    ~MyServo();
    void open() override;
    void close() override;
    void setupMotor() override;
    bool getState() override;
};
