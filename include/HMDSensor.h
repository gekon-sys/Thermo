#pragma once
#include "config.h"

class HMDSensor : public Sensor
{
private:
    static uint8_t counter;
    static HMDSensor* instances[];

    uint8_t dataPin;
    uint8_t powerPin;
    const char *name;
    bool status;
public:
    HMDSensor(uint8_t powerPin, const char *name,uint8_t dataPin);
    ~HMDSensor() override;
    bool setupSensor();
    float read() override;
    bool checkData(float &data) override;
    uint8_t getPin() override;
    const char *getName() const override;
    virtual bool getStatus() override;
    static uint8_t getCounter();
    static HMDSensor* getInstance(int index);

};