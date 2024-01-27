#include "Config.h"

Fan::Fan(uint8_t pin, bool state = false) : pin{FAN_PIN_CONFIG}
{
}

Fan::~Fan()
{
}

void Fan::setupMotor()
{
    pinMode(FAN_PIN_CONFIG, OUTPUT);
    // Serial.println("FAN STATUS...OK");
    // Serial.print("FAN PIN...");
    // Serial.println(pin);   
}

void Fan::open()
{
    digitalWrite(pin, HIGH);
    state = true;
}

void Fan::close()
{
    digitalWrite(pin, LOW);
    state = false;
}

bool Fan::getState()
{
    return state;
}