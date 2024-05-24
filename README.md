# Automated Greenhouse Project

## Overview

This project is an automated greenhouse system designed for efficient plant cultivation. The core of the system is based on the ESP32 microcontroller (specifically, ESP32-DevKitC-02), capable of reading data from various sensors and controlling actuators. The project leverages a modular and scalable architecture, allowing users to easily extend and customize the system with additional sensors and actuators.

## Features

- **Modular Design:** The project is built upon a modular architecture, utilizing interface classes for sensors and actuators. This design allows for seamless scalability without the need to rewrite code for each new component.

- **Scalability:** Easily expand the system by adding new sensors or actuators. The scalability is achieved through a configuration file (`config.h`), where users can define new objects and corresponding functions for additional components.

- **Firebase Integration:** The system sends sensor data to a Firebase server for remote monitoring and data analysis.

## Getting Started

Follow these steps to set up and customize the automated greenhouse system:

1. **Hardware Setup:** Connect the ESP32 microcontroller, sensors (DHT11, soil moisture), actuators (water pump, fan, servo), and any additional components as per the provided circuit diagram.

2. **Configuration:** Open the `config.h` file to add new sensor or actuator objects. Define the required functions for each component.

3. **Firebase Setup:** Configure your Firebase credentials in the code to enable data transmission to the Firebase server.

4. **Upload Code:** Upload the code to the ESP32 microcontroller using the Arduino IDE or your preferred development environment.

## Adding New Components

To add new sensors or actuators to the system:

1. Create a new object derived from the corresponding interface class (`Sensor` or `Motor`).
2. Define the necessary functions for the new component in the `config.h` file.

```cpp
// Example for adding a new DHT11 sensor
// DHT Sensor parameters Config.h
extern const uint8_t DHT_TYPE_CONFIG;
extern const uint8_t DHT_DATA_PIN_CONFIG;
extern float DHT_THRESHOLD;
extern float OFFSET;
extern DHTSensor dhtSensor1;

// DHT Sensor parameters Config.cpp
const uint8_t DHT_TYPE_CONFIG = DHT11;
const uint8_t DHT_DATA_PIN_CONFIG{6};
float DHT_THRESHOLD = 26;
float OFFSET{1};
DHTSensor dhtSensor1(DHT_DATA_PIN_CONFIG, DHT_TYPE_CONFIG);

// Add the methods inside function. 
float readCheckDhtData()
{
    float currentTemp = dhtSensor1.read();
    while (1) // Run the loop unti the data is valid.
    {
        if (dhtSensor1.checkData(currentTemp))
            break;
        else
        {
            delay(1000);
            currentTemp = dhtSensor1.read();
        }
    }
    // Serial.println(currentTemp);
    return currentTemp;
}
