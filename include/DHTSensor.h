#pragma once

#include "Sensor.h"
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

class DHTSensor : public Sensor
{
private:
    DHT_Unified dht;
    uint8_t DHT_TYPE;
    uint8_t pin;
    sensor_t sensor;
    sensors_event_t event;

public:
    DHTSensor(uint8_t pin, uint8_t DHT_TYPE);
    ~DHTSensor() override;
    float read() override;
    bool isCloseToThreshold(float &data) override;
    bool checkData(float &data) override;
    uint8_t returnPin() override;
    void setupDHT();
};
