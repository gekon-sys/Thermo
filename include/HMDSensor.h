#pragma once
#include "Config.h"

class HMDSensor : public Sensor
{
    private:
        uint8_t dataPin;
        uint8_t powerPin;
    public:
        HMDSensor(uint8_t powerPin, uint8_t dataPin);
        ~HMDSensor() override;
        float read() override;
        bool isCloseToThreshold(float &data) override;
        bool checkData(float &data) override;
        void setupHMD();
        uint8_t returnPin() override;
        
};