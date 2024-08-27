#ifndef _CONFIG_H_
#define _CONFIG_H_

// Include necessary libraries
#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

// Include custom sensor and actuator definitions
#include "DHTSensor.h"
#include "HMDSensor.h"
#include "fan.h"
#include "pump.h"
#include "server.h"
#include "servo.h"

// Define constants for the number of sensors and motors
#define SENSORS_NUM 2
#define MOTORS_NUM 2

// Define GPIO pins for RGB LED
#define PIN_RED 4    // GPIO4
#define PIN_GREEN 5  // GPIO5
#define PIN_BLUE 6   // GPIO6

// Function prototypes
void logError(const Sensor &sensor);
#define ERROR_LOG(sensorInstance) logError(sensorInstance)

// Data structures for sensor data
typedef struct TemperatureData {
    float tempArray[SENSORS_NUM]; // Array to hold temperature data
} TemperatureData;

typedef struct MoistureData {
    float moistArray[SENSORS_NUM]; // Array to hold moisture data
} MoistureData;

// Extern declarations for sensor and actuator data
extern TemperatureData temperature;
extern MoistureData moisture;

// DHT Sensor parameters
extern float DHT_THRESHOLD;  // Threshold for temperature
extern float HYSTERISIS;     // Hysteresis value for temperature control
extern uint8_t dhtCount;     // Number of DHT sensors

// DHT Sensor configurations
extern const uint8_t DHT_1_TYPE_CONFIG;       // Type of DHT sensor 1
extern const uint8_t DHT_1_DATA_PIN_CONFIG;   // Data pin for DHT sensor 1
extern DHTSensor dht_sensor_1;                 // Instance of DHT sensor 1

extern const uint8_t DHT_2_TYPE_CONFIG;       // Type of DHT sensor 2
extern const uint8_t DHT_2_DATA_PIN_CONFIG;   // Data pin for DHT sensor 2
extern DHTSensor dht_sensor_2;                 // Instance of DHT sensor 2

/*
    Enter your DHT sensor attributes above
*/

// HMD Sensor parameters
extern float HMD_THRESHOLD; // Threshold for humidity
extern uint8_t hmdCount;    // Number of HMD sensors

// HMD Sensor configurations
extern const uint8_t HMD_1_DATA_PIN_CONFIG;   // Data pin for HMD sensor 1
extern const uint8_t HMD_1_POWER_PIN_CONFIG;  // Power pin for HMD sensor 1
extern HMDSensor hmd_sensor_1;                // Instance of HMD sensor 1

/*
    Enter your HMD sensor attributes above
*/

// Fan motor parameters
extern bool fans_on;     // Flag indicating if fans are on
extern uint8_t fanCount; // Number of fan motors

// Fan Motor configurations
extern const uint8_t FAN_1_PIN_CONFIG; // Pin configuration for Fan Motor 1
extern Fan fan_motor_1;                // Instance of Fan Motor 1

/*
    Enter your Fan Motor attributes above
*/

// Pump motor parameters
extern bool pumps_on;   // Flag indicating if pumps are on
extern uint8_t pumpCount; // Number of pump motors

// Pump Motor configurations
extern const uint8_t PUMP_1_PIN_CONFIG; // Pin configuration for Pump Motor 1
extern Pump pump_motor_1;              // Instance of Pump Motor 1

/*
    Enter your Pump Motor attributes above
*/

// Servo motor parameters
extern bool servos_on; // Flag indicating if servos are on
extern uint8_t servoCount; // Number of servo motors

// Servo Motor configurations
extern const uint8_t SERVO_1_PIN_CONFIG; // Pin configuration for Servo Motor 1
extern WindowMotor servo_motor_1;       // Instance of Servo Motor 1

/*
    Enter your Servo Motor attributes above
*/

// WiFi credentials
extern const char *SSID;     // WiFi SSID
extern const char *PASSWORD; // WiFi password

// Network server configurations
extern const IPAddress local_IP;  // Local IP address
extern const IPAddress gateway;   // Gateway IP address
extern const IPAddress subnet;    // Subnet mask
extern WebServer server;          // Web server instance

// Buffers for data handling
extern char XML[]; // Buffer for XML data
extern char buf[]; // Buffer for general data

// FreeRTOS task handles
extern TaskHandle_t network_handle;         // Task handle for network operations
extern TaskHandle_t read_sensors_handle;    // Task handle for reading sensors
extern TaskHandle_t control_actuators_handle; // Task handle for controlling actuators
extern TaskHandle_t control_servo_handle;   // Task handle for controlling servos

// Queue configuration
extern const uint8_t msg_dht_queue_len; // Length of DHT data queue
extern const uint8_t msg_hmd_queue_len; // Length of HMD data queue
extern QueueHandle_t msg_dht_queue;     // Queue handle for DHT data
extern QueueHandle_t msg_hmd_queue;     // Queue handle for HMD data

// Valid ADC Pins
extern const uint8_t validADCpins[];
extern const size_t validADCpinCount;

#endif // _CONFIG_H_
