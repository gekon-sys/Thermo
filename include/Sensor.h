#pragma once
#include <cstdint>

class Sensor
{
public:
    virtual bool setupSensor() = 0;
    virtual float read() = 0;
    virtual bool checkData(float &data) = 0;
    virtual uint8_t getPin() = 0;
    virtual ~Sensor() = default;
    virtual const char *getName() const = 0;
    virtual bool getStatus() = 0;
    // virtual void logError(const char* name) = 0;
};