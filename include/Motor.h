#pragma once
#include <cstdint>

class Motor
{
    public:
        virtual void open() = 0;
        virtual void close() = 0;
        virtual void setupMotor() = 0;
        virtual bool getState() = 0;
        virtual ~Motor() = default;
};