#include "config.h"

uint8_t Fan::counter{0};
Fan* Fan::instances[MOTORS_NUM] = {nullptr};

Fan::Fan(uint8_t pin, const char* name, bool state = false) : pin{pin}, name{name} {
    instances[counter++] = this;
}

Fan::~Fan() {
}

bool Fan::setupMotor() {
    pinMode(pin, OUTPUT);
    return true;   
}
void Fan::open() {
    digitalWrite(pin, HIGH);
    state = true;
}

void Fan::close() {
    digitalWrite(pin, LOW);
    state = false;
}

bool Fan::getState() {
    return state;
}

const char* Fan::getName(){
    return name;
}

uint8_t Fan::getPin(){
    return pin;
}

uint8_t Fan::getCounter() {
    return counter;
}

Fan* Fan::getInstance(int index) {
    return (index > counter ? nullptr : instances[index]);
}