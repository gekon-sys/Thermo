#pragma once

#include "Sensor.h"
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

class DHTSensor : public Sensor
{
private:
    static uint8_t counter;
    static DHTSensor* instances[];

    DHT_Unified dht;
    uint8_t DHT_TYPE;
    uint8_t pin;
    sensor_t sensor;
    sensors_event_t event;
    const char *name;
    bool status;

public:
    DHTSensor(uint8_t pin, const char *name, uint8_t DHT_TYPE);
    ~DHTSensor() override;
    bool setupSensor() override;
    float read() override;
    bool checkData(float &data) override;
    uint8_t getPin() override;
    const char *getName() const override;
    virtual bool getStatus() override;
    // static 
    static uint8_t getCounter();
    static DHTSensor* getInstance(int index);
};
