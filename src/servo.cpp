#include "config.h"

uint8_t WindowMotor::counter{0};
WindowMotor *WindowMotor::instances[MOTORS_NUM] = {nullptr};

WindowMotor::WindowMotor(uint8_t pin, const char *name, bool state) : pin{pin}, name{name} {
    instances[counter++] = this;
}

WindowMotor::~WindowMotor() {}

bool WindowMotor::setupMotor() {
    pinMode(pin, OUTPUT);
    servo.attach(pin);
    return true;
}

void WindowMotor::open() {
    Serial.println("Window open");

    for (int i{0}; i < 180; i++) {
        servo.write(i);
        vTaskDelay(15 / portTICK_PERIOD_MS);
    }
    state = true;
}

void WindowMotor::close() {
    Serial.println("Window close");

    for (int i{180}; i > 0; i--) {
        servo.write(i);
        vTaskDelay(15 / portTICK_PERIOD_MS);
    }
    state = false;
}

bool WindowMotor::getState() {
    return state;
}

uint8_t WindowMotor::getPin() {
    return pin;
}

const char *WindowMotor::getName() {
    return name;
}

uint8_t WindowMotor::getCounter() {
    return counter;
}

WindowMotor *WindowMotor::getInstance(int index) {
    return (index > counter ? nullptr : instances[index]);
}