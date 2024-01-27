#include "Config.h"

DHTSensor::DHTSensor(uint8_t pin, const uint8_t DHT_TYPE) : pin{pin}, DHT_TYPE{DHT_TYPE}, dht{pin, DHT_TYPE}
{
}

DHTSensor::~DHTSensor()
{
}

bool DHTSensor::isCloseToThreshold(float &data)
{
    if (data >= DHT_THRESHOLD)
        return true;
    if (data <= (DHT_THRESHOLD - OFFSET))
        return false;
    return true;
}

float DHTSensor::read()
{
    sensor_t sensor;
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    return event.temperature;
}

uint8_t DHTSensor::returnPin()
{
    return pin;
}

void DHTSensor::setupDHT()
{
    // Serial.println("DHT11 Sensor installation...");
    pinMode(DHT_DATA_PIN_CONFIG, INPUT);
    dht.begin();
    dht.temperature().getSensor(&sensor);
    while (isnan(read()))
    {
        digitalWrite(0, HIGH);
        // Serial.println("DHT11 SETUP STATUS: ERROR. Check the wiring/Sensor failure");
        delay(1000);
    }
    digitalWrite(0, LOW);
    digitalWrite(1, HIGH);
    // Serial.println("DHT11 STATUS...OK");
    // Serial.print("DHT TYPE...");
    // Serial.println(DHT_TYPE_CONFIG);
    // Serial.print("DHT SENSOR DATA PIN...");
    // Serial.println(returnPin());
}

bool DHTSensor::checkData(float &data)
{
    if (!isnan(data))
    {
        digitalWrite(0, LOW);
        digitalWrite(1, HIGH);
        return true;
    }
    else
    {
        digitalWrite(1, LOW);
        digitalWrite(0, HIGH);
        return false;
    }
}
