#include "Config.h"

HMDSensor::HMDSensor(uint8_t powerPin, uint8_t dataPin) : powerPin{HMD_POWER_PIN_CONFIG}, dataPin{HMD_DATA_PIN_CONFIG}
{
}

HMDSensor::~HMDSensor()
{
}

float HMDSensor::read()
{
    digitalWrite(HMD_POWER_PIN_CONFIG, HIGH);
    delay(10);
    float moisture = analogRead(HMD_DATA_PIN_CONFIG);
    moisture = map(moisture, 22, 4000, 0, 100);
    digitalWrite(HMD_POWER_PIN_CONFIG, LOW);
    return moisture;
}

void HMDSensor::setupHMD()
{
    // Serial.println("HMD Sensor installation...");
    pinMode(HMD_POWER_PIN_CONFIG, OUTPUT);
    pinMode(HMD_DATA_PIN_CONFIG, INPUT);
    // Serial.println("HMD STATUS...OK");
    // Serial.print("HMD SENSOR DATA PIN...");
    // Serial.println(returnPin());
}

bool HMDSensor::isCloseToThreshold(float &data)
{
    if (data >= HMD_THRESHOLD)
        return true;
    if(data <= (HMD_THRESHOLD - OFFSET))
        return false;
    return true;
}

bool HMDSensor::checkData(float &data)
{
    if(data > 100 || data < 0)
        return false;
    else
        return true;
}

uint8_t HMDSensor::returnPin()
{
    return dataPin;
}