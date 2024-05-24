#pragma once
#include "Motor.h"
class Pump : public Motor
{
private:
    const char *name;
    uint8_t pin;
    bool state;

public:
    Pump(uint8_t pin, const char* name,  bool state = false);
    ~Pump() override;
    void open() override;
    void close() override;
    void setupMotor() override;
    const char* getName();
    bool getState() override;
};