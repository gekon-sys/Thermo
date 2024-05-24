#include "Config.h"

float currentTemp{0};
float currentMoist{0};

// Wifi credentials
const char *SSID = "";
const char *PASSWORD = "";
const int MAX_CONNECTION_ATTEMPTS{10};

// Firebase
const char *API_KEY = "AIzaSyDoklSU903Y_uCT_PPpvXtuKkqP-_eS2vg";
const char *DATABASE_URL = "https://automated-greenhouse-6ca23-default-rtdb.firebaseio.com/";


// DHT Sensor parameters
const uint8_t DHT_TYPE_CONFIG = DHT11;
const uint8_t DHT_DATA_PIN_CONFIG{6};
float DHT_THRESHOLD = 26;
float OFFSET{1};
DHTSensor dhtSensor1(DHT_DATA_PIN_CONFIG, DHT_TYPE_CONFIG);

//  HMD Sensor parameters
float HMD_THRESHOLD = 60;
const uint8_t HMD_DATA_PIN_CONFIG = 3;
const uint8_t HMD_POWER_PIN_CONFIG = 7;
HMDSensor hmdSensor1(HMD_POWER_PIN_CONFIG, HMD_DATA_PIN_CONFIG);

//  Fan parameters
const uint8_t FAN_PIN_CONFIG{4};
Fan fanMotor1(FAN_PIN_CONFIG, false);

// Pump parameters
const uint8_t PUMP_PIN_CONFIG{5};
Pump pumpMotor1{PUMP_PIN_CONFIG, "Pump1"};

// Servo parameters
const uint8_t SERVO_PIN_CONFIG{19};
const uint8_t HZ_CONFIG{60};
const int MIN_PULSE_WIDTH_CONFIG{500};
const int MAX_PULSE_WIDTH_CONFIG{2500};
MyServo servoMotor1(SERVO_PIN_CONFIG, HZ_CONFIG, MIN_PULSE_WIDTH_CONFIG, MAX_PULSE_WIDTH_CONFIG);