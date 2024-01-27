#pragma once
#include <cstdint>

class Sensor
{
public:
    virtual float read() = 0;
    virtual bool isCloseToThreshold(float &data) = 0;
    virtual bool checkData(float &data) = 0;
    virtual uint8_t returnPin() = 0;
    virtual ~Sensor() = default;
};