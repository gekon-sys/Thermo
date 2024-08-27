#include "config.h"

// Function to log errors for sensors
void logError(const Sensor &sensor) {
    Serial.print("ERROR WITH MODULE: ");
    Serial.print(sensor.getName());
    Serial.println(" CHECK WIRING/SENSOR FAILURE");
}

// Initialize global data structures for temperature and moisture
TemperatureData temperature;
MoistureData moisture;

// DHT Sensor 1 Initialization
const uint8_t DHT_1_TYPE_CONFIG{DHT11};            // Type of DHT sensor 1
const uint8_t DHT_1_DATA_PIN_CONFIG{1};            // Data pin for DHT sensor 1
DHTSensor dht_sensor_1(DHT_1_DATA_PIN_CONFIG, "DHT_SENSOR_1", DHT_1_TYPE_CONFIG);

// DHT Sensor 2 Initialization
const uint8_t DHT_2_TYPE_CONFIG{DHT11};            // Type of DHT sensor 2
const uint8_t DHT_2_DATA_PIN_CONFIG{2};            // Data pin for DHT sensor 2
DHTSensor dht_sensor_2(DHT_2_DATA_PIN_CONFIG, "DHT_SENSOR_2", DHT_2_TYPE_CONFIG);

/*
    Enter your DHT sensor attributes above
*/

// DHT Sensor parameters
float DHT_THRESHOLD{25};     // Temperature threshold for DHT sensors
float HYSTERISIS{0.5};       // Hysteresis for temperature control
uint8_t dhtCount = DHTSensor::getCounter(); // Number of DHT sensors

// HMD Sensor 1 Initialization
const uint8_t HMD_1_DATA_PIN_CONFIG{20};          // Data pin for HMD sensor 1
const uint8_t HMD_1_POWER_PIN_CONFIG{37};         // Power pin for HMD sensor 1
HMDSensor hmd_sensor_1(HMD_1_POWER_PIN_CONFIG, "HMD_SENSOR_1", HMD_1_DATA_PIN_CONFIG);

/*
    Enter your HMD sensor attributes above
*/

// HMD Sensor parameters
float HMD_THRESHOLD{60};    // Humidity threshold for HMD sensors
uint8_t hmdCount = HMDSensor::getCounter(); // Number of HMD sensors

// Fan Motor 1 Initialization
const uint8_t FAN_1_PIN_CONFIG{12};                // Pin configuration for Fan Motor 1
Fan fan_motor_1(FAN_1_PIN_CONFIG, "FAN_1", false); // Fan motor instance

/*
    Enter your Fan Motor attributes above
*/

// Fan parameters
bool fans_on;             // Flag indicating if fans are on
uint8_t fanCount = Fan::getCounter(); // Number of fan motors

// Pump Motor 1 Initialization
const uint8_t PUMP_1_PIN_CONFIG{13};                // Pin configuration for Pump Motor 1
Pump pump_motor_1{PUMP_1_PIN_CONFIG, "PUMP_1"};     // Pump motor instance

/*
    Enter your Pump Motor attributes above
*/

// Pump parameters
bool pumps_on;           // Flag indicating if pumps are on
uint8_t pumpCount = Pump::getCounter(); // Number of pump motors

// Servo Motor 1 Initialization
const uint8_t SERVO_1_PIN_CONFIG{45};                // Pin configuration for Servo Motor 1
WindowMotor servo_motor_1(SERVO_1_PIN_CONFIG, "WINDOW_1"); // Servo motor instance

/*
    Enter your Servo Motor attributes above
*/

// Servo parameters
bool servos_on;           // Flag indicating if servos are on
uint8_t servoCount = WindowMotor::getCounter(); // Number of servo motors

// Wifi credentials
const char *SSID = "COSMOTE-458524";   // WiFi SSID
const char *PASSWORD = "ehm9mk3b5k4xk1ex"; // WiFi password

// Server configuration
const IPAddress local_IP(192, 168, 1, 184);   // Local IP address
const IPAddress gateway(192, 168, 1, 1);      // Gateway IP address
const IPAddress subnet(255, 255, 0, 0);       // Subnet mask
WebServer server(80);                        // Web server instance

// Buffers for data handling
char XML[2048];  // Buffer for XML data
char buf[32];    // Buffer for general data

// FreeRTOS task handles
TaskHandle_t network_handle;          // Handle for network task
TaskHandle_t read_sensors_handle;     // Handle for reading sensors task
TaskHandle_t control_actuators_handle; // Handle for controlling actuators task
TaskHandle_t control_servo_handle;    // Handle for controlling servos task

// Queue configuration
const uint8_t msg_dht_queue_len{3};   // Length of DHT data queue
const uint8_t msg_hmd_queue_len{3};   // Length of HMD data queue
QueueHandle_t msg_dht_queue;          // Queue handle for DHT data
QueueHandle_t msg_hmd_queue;          // Queue handle for HMD data

// Valid ADC Pins
const uint8_t validADCpins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20}; // Example ADC pins for ESP32
const size_t validADCpinCount = sizeof(validADCpins) / sizeof(validADCpins[0]);