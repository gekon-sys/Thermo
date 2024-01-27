#pragma once

#include <WiFi.h>
#include "DHTSensor.h"
#include "HMDSensor.h"
#include "Fan.h"
#include "Pump.h"
#include "MyServo.h"

extern float currentTemp;
extern float currentMoist;

// Wifi credentials
extern const char *SSID;
extern const char *PASSWORD;
extern const int MAX_CONNECTION_ATTEMPTS;

// Firebase
extern const char *API_KEY;
extern const char *DATABASE_URL;

// DHT Sensor parameters
extern const uint8_t DHT_TYPE_CONFIG;
extern const uint8_t DHT_DATA_PIN_CONFIG;
extern float DHT_THRESHOLD;
extern float OFFSET;
extern DHTSensor dhtSensor1;


// HMD Sensor parameters
extern HMDSensor hmdSensor1;
extern float HMD_THRESHOLD;
extern const uint8_t HMD_DATA_PIN_CONFIG;
extern const uint8_t HMD_POWER_PIN_CONFIG;

// Fan motor parameters
extern const uint8_t FAN_PIN_CONFIG;
extern Fan fanMotor1;

// Pump motor parameters
extern const uint8_t PUMP_PIN_CONFIG;
extern Pump pumpMotor1;

// Servo motor parameters
extern const uint8_t SERVO_PIN_CONFIG;
extern const uint8_t HZ_CONFIG;
extern const int MIN_PULSE_WIDTH_CONFIG;
extern const int MAX_PULSE_WIDTH_CONFIG;
extern MyServo servoMotor1;