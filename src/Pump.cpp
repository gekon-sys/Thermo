#include "Config.h"

Pump::Pump(uint8_t pin, const char *name, bool state) : pin{PUMP_PIN_CONFIG}, name{name}
{
}

Pump::~Pump()
{
}

void Pump::setupMotor()
{
    pinMode(PUMP_PIN_CONFIG, OUTPUT);
    // Serial.println("PUMP STATUS...OK");
    // Serial.print("PUMP PIN...");
    // Serial.println(pin);
    // Serial.println(Pump::getName());
}

void Pump::open()
{
    digitalWrite(PUMP_PIN_CONFIG, HIGH);
    state = true;
}

void Pump::close()
{
    digitalWrite(PUMP_PIN_CONFIG, LOW);
    state = false;
}

const char* Pump::getName()
{
    return name;
}

bool Pump::getState()
{
    return state;
}