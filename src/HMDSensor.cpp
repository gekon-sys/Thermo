#include "config.h"

uint8_t HMDSensor::counter{0};
HMDSensor *HMDSensor::instances[SENSORS_NUM] = {nullptr};

HMDSensor::HMDSensor(uint8_t powerPin, const char *name, uint8_t dataPin) : powerPin{powerPin}, name{name}, dataPin{dataPin} {
    instances[counter++] = this;
}

HMDSensor::~HMDSensor() {
}

bool HMDSensor::setupSensor() {
    for (size_t i = 0; i < validADCpinCount; i++) {
        if (dataPin == validADCpins[i]) {
            pinMode(dataPin, INPUT);
            pinMode(powerPin, OUTPUT);
            float value = read();
            while (!checkData(value)) {
                Serial.print("HMD SETUP STATUS: MODULE: ");
                Serial.print(getName());
                Serial.println(" ERROR. Check the wiring/Sensor failure");
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
            if (value >= HMD_THRESHOLD)
                pumps_on = true;
            else
                pumps_on = false;
            return true;
        }
    }
    Serial.printf("In HMDSensor setup() %hhu is not and ADC pin. Check data sheet or the validADCpins list.\n", dataPin);
    return false;
}

float HMDSensor::read() {
    digitalWrite(powerPin, HIGH);
    float moisture = analogRead(dataPin);
    moisture = (moisture / 4095.0) * 100.0;
    digitalWrite(powerPin, LOW);
    return moisture;
}

bool HMDSensor::checkData(float &data) {
    if (data > 100 || data < 0) {
        status = false;
        return status;
    } else {
        status = true;
        return status;
    }
}

uint8_t HMDSensor::getPin() {
    return dataPin;
}

const char *HMDSensor::getName() const {
    return name;
}

uint8_t HMDSensor::getCounter() {
    return counter;
}

bool HMDSensor::getStatus() {
    return status;
}

HMDSensor *HMDSensor::getInstance(int index) {
    return (index > counter ? nullptr : instances[index]);
}