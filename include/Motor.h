#pragma once
#include <cstdint>

class Motor {
public:
    virtual ~Motor() = default;

    virtual void open() = 0;
    virtual void close() = 0;
    virtual bool setupMotor() = 0;
    virtual bool getState() = 0;
    virtual uint8_t getPin() = 0;
    virtual const char *getName() = 0;
};