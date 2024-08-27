#pragma once

#include "motor.h"
#include <ESP32Servo.h>

class WindowMotor : public Motor {
private:
    static uint8_t counter;
    static WindowMotor *instances[];

    uint8_t pin;
    bool state;
    Servo servo;
    const char* name;

public:
    WindowMotor(uint8_t pin, const char* name, bool state = false);
    ~WindowMotor() override;
    bool setupMotor() override;
    void open() override;
    void close() override;
    bool getState() override;
    uint8_t getPin() override;
    const char *getName() override;

    static uint8_t getCounter();
    static WindowMotor *getInstance(int index);
};
