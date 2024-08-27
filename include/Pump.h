#pragma once
#include "motor.h"
class Pump : public Motor
{
private:
    static uint8_t counter;
    static Pump* instances[];

    const char *name;
    uint8_t pin;
    bool state;

public:
    Pump(uint8_t pin, const char* name);
    ~Pump() override;
    bool setupMotor() override;
    void open() override;
    void close() override;
    bool getState() override;
    uint8_t getPin() override;
    const char* getName();

    static uint8_t getCounter();
    static Pump* getInstance(int index);
};