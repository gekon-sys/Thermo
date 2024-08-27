#include "config.h"

/**
 * @class DHTSensor
 * @brief Handles DHT sensor operations including initialization, reading, and validation.
 */

uint8_t DHTSensor::counter{0};
DHTSensor *DHTSensor::instances[SENSORS_NUM] = {nullptr};

DHTSensor::DHTSensor(uint8_t pin, const char *name, const uint8_t DHT_TYPE) : pin{pin}, name{name}, DHT_TYPE{DHT_TYPE}, dht{pin, DHT_TYPE} {
    instances[counter++] = this;
}

DHTSensor::~DHTSensor() {
}

float DHTSensor::read() {
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    return event.temperature;
}

uint8_t DHTSensor::getPin() {
    return pin;
}

bool DHTSensor::setupSensor() {
    pinMode(pin, INPUT);
    dht.begin();
    dht.temperature().getSensor(&sensor);
    float temperature = read();

    if (isnan(temperature)) {
        Serial.printf("DHT11 SETUP STATUS: MODULE: %s ERROR. Check the wiring/Sensor failure\n", getName());
        return false;
    }

    if (temperature >= DHT_THRESHOLD) {
        fans_on = false;
        servos_on = false;
    } else {
        fans_on = true;
        servos_on = true;
    }
    return true;
}

bool DHTSensor::checkData(float &data) {
    status = !isnan(data);  // if data are not nan
    return status;
}

const char *DHTSensor::getName() const {
    return name;
}

uint8_t DHTSensor::getCounter() {
    return counter;
}

bool DHTSensor::getStatus() {
    return status;
}

DHTSensor *DHTSensor::getInstance(int index) {
    return (index >= counter ? nullptr : instances[index]);
}
