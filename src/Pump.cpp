#include "config.h"

uint8_t Pump::counter{0};
Pump* Pump::instances[MOTORS_NUM] = {nullptr};

Pump::Pump(uint8_t pin, const char* name) : pin{pin}, name{name} {
    instances[counter++] = this;
}

Pump::~Pump() {
}

bool Pump::setupMotor() {
    pinMode(pin, OUTPUT);
    return true;
}

void Pump::open() {
    digitalWrite(pin, HIGH);
    state = true;
}

void Pump::close() {
    digitalWrite(pin, LOW);
    state = false;
}

bool Pump::getState() {
    return state;
}

uint8_t Pump::getPin(){
    return pin;
}

const char* Pump::getName() {
    return name;
}

uint8_t Pump::getCounter() {
    return counter;
}

Pump* Pump::getInstance(int index) {
    return (index > counter ? nullptr : instances[index]);
}
