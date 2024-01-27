#include "Config.h"

MyServo::MyServo(uint8_t pin, uint8_t hz, int minPulse, int maxPulse, bool state) : pin{SERVO_PIN_CONFIG}, hz{HZ_CONFIG}, minPulse{MIN_PULSE_WIDTH_CONFIG}, maxPulse{MAX_PULSE_WIDTH_CONFIG} {}

MyServo::~MyServo() {}

void MyServo::setupMotor()
{
    servo.setPeriodHertz(hz);
    servo.attach(pin);
    pinMode(SERVO_PIN_CONFIG, OUTPUT);
    // Serial.println("SERVO STATUS...OK");
    // Serial.print("SERVO PIN...");
    Serial.println(pin);
}

void MyServo::open()
{
    // Serial.println("Temperature above threshold.Opening window");
    for (int i{0}; i < 180; i++)
    {
        servo.write(i);
        delay(30);
    }
    state = true;
}

void MyServo::close()
{
    // Serial.println("Temperature below threshold.Closing window");
    for (int i{180}; i > 0; i--)
    {
        servo.write(i);
        delay(30);
    }
    state = false;
}

bool MyServo::getState()
{
    return state;
}