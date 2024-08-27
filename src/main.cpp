#include "config.h"

static const BaseType_t app_cpu = 1;
static const BaseType_t pro_cpu = 0;

/**
 * @brief Manages WiFi connectivity and handles incoming client requests.
 *
 * This task connects to WiFi, runs a server, and handles client connections.
 * It also periodically re-establishes the WiFi connection if needed.
 *
 * @param args Task arguments (not used in this implementation).
 */
void network_task(void *args) {
    connectToWifi();
    runServer();
    while (1) {
        server.handleClient();
        connectToWifi(); // Mantain network connection

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Reads data from DHT and HMD sensors and sends it to the corresponding queues.
 *
 * This task reads temperature and humidity data from DHT and HMD sensors.
 * It fills arrays with the sensor data and sends them to the message queues for processing.
 *
 * @param args Task arguments (not used in this implementation).
 */
void read_sensors(void *args) {
    size_t maxCount = max(dhtCount, hmdCount);
    while (1) {
        // Read the data from the DHT sensors
        for (size_t i = 0; i < maxCount; i++) {
            if (i < dhtCount) {
                float temp;
                DHTSensor *dhtSensor = DHTSensor::getInstance(i);
                temp = dhtSensor->read();

                // Serial.printf("Reading DHT Sensors: Sensor[%d]: %f\n", i, temp); // Uncomment for raw data display

                if (dhtSensor->checkData(temp)) {
                    // Fill the array with correct dht data
                    temperature.tempArray[i] = temp;
                } else {
                    temperature.tempArray[i] = NAN;
                    ERROR_LOG(*dhtSensor);
                }
            }
            if (i < hmdCount) {
                float moist;
                HMDSensor *hmdSensor = HMDSensor::getInstance(i);
                moist = hmdSensor->read();

                // Serial.printf("Reading HMD Sensors: Sensor[%d]: %f\n", i, moist); // Uncomment for raw data display

                if (hmdSensor->checkData(moist)) {
                    // Fill the array with correct hmd data
                    moisture.moistArray[i] = moist;
                } else {
                    moisture.moistArray[i] = NAN;
                    ERROR_LOG(*hmdSensor);
                }
            }
        }
        // When the array is filled, send it to the queue
        if (xQueueSend(msg_dht_queue, (void *)&temperature, 10) != pdTRUE) {
            Serial.println("Temperature Queue full");
        }

        // When the array is filled, send it to queue
        if (xQueueSend(msg_hmd_queue, (void *)&moisture, 10) != pdTRUE) {
            Serial.println("Humidity Queue full");
        }
        vTaskDelay(2500 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Controls actuators based on sensor data from the queues.
 *
 * This task processes temperature and humidity data from the message queues.
 * It adjusts the state of fans, windows, and pumps based on the average sensor readings.
 * To avoid switching states between on and off of the fans and the windows a HYSTERISIS has been added.
 *
 * @param args Task arguments (not used in this implementation).
 */
void control_actuators(void *args) {
    while (1) {
        if (uxQueueMessagesWaiting(msg_dht_queue) > 0) { // Only if the queue is not empty, get the data
            if (xQueueReceive(msg_dht_queue, (void *)&temperature, portMAX_DELAY) == pdTRUE) {
                float sum = 0;
                Serial.println();
                for (size_t i = 0; i < dhtCount; i++) {
                    Serial.printf("Temperature[%d]: %.2f\n", i, temperature.tempArray[i]);
                    sum += temperature.tempArray[i];
                }

                float dht_mean;
                dht_mean = sum / dhtCount; // Mean temperature inside the greenhouse
                Serial.print("DHT MEAN: ");
                Serial.println(dht_mean);
                if (dht_mean >= DHT_THRESHOLD + HYSTERISIS) { // dht_mean >= 25.5
                    // Start the fans to cooldown the house.
                    if (!fans_on) {
                        for (size_t i = 0; i < fanCount; i++) {
                            Fan::getInstance(i)->open();
                        }
                        fans_on = true;
                        Serial.println("Fans active");
                    }
                    // Open the windows
                    if (!servos_on) {
                        for (size_t i = 0; i < servoCount; i++) {
                            WindowMotor::getInstance(i)->open();
                        }
                        servos_on = true;
                        Serial.println("Windows open");
                    }
                } else if (dht_mean <= DHT_THRESHOLD - HYSTERISIS) { // dht_mean <= 24.5
                    // Stop the fans to heat up the house.
                    if (fans_on) {
                        for (size_t i = 0; i < fanCount; i++) {
                            Fan::getInstance(i)->close();
                        }
                        fans_on = false;
                        Serial.println("Fans deactive");
                    }
                    // Close the windows
                    if (servos_on) {
                        for (size_t i = 0; i < servoCount; i++) {
                            WindowMotor::getInstance(i)->close();
                        }
                        servos_on = false;
                        Serial.println("Windows open");
                    }
                }
            } else {
                Serial.println("Didn't receive the DHT data");
            }
        }

        if (uxQueueMessagesWaiting(msg_hmd_queue) > 0) { // Only if the queue is not empty, get the data
            if (xQueueReceive(msg_hmd_queue, (void *)&moisture, portMAX_DELAY) == pdTRUE) {
                float sum = 0;
                for (size_t i = 0; i < hmdCount; i++) {
                    Serial.printf("Humidity[%d]: %.2f\n", i, moisture.moistArray[i]);
                    sum += moisture.moistArray[i];
                }
                float hmd_mean;
                hmd_mean = sum / hmdCount;

                switch (hmd_mean >= HMD_THRESHOLD) {
                case true:
                    // Close pumps
                    if (pumps_on) {
                        for (size_t i = 0; i < pumpCount; i++) {
                            Pump::getInstance(i)->close();
                        }
                        pumps_on = false;
                        Serial.println("Pumps deactive");
                    }
                    break;
                case false:
                    // Open pumps
                    if (!pumps_on) {
                        for (size_t i = 0; i < pumpCount; i++) {
                            Pump::getInstance(i)->open();
                        }
                        pumps_on = true;
                        Serial.println("Pumps active");
                    }
                    break;
                }
            } else {
                Serial.println("Didnt receive the HMD data");
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Sets the color of RGB LEDs.
 *
 * This function adjusts the RGB LED colors by setting the values for red, green, and blue.
 *
 * @param R Red color intensity (0-255).
 * @param G Green color intensity (0-255).
 * @param B Blue color intensity (0-255).
 */
void setColor(int R, int G, int B) {
    analogWrite(PIN_RED, R);
    analogWrite(PIN_GREEN, G);
    analogWrite(PIN_BLUE, B);
}

/**
 * @brief Initializes hardware components and creates tasks.
 *
 * This function sets up the pins, initializes sensors and actuators,
 * given in the `config.h` and `config.cpp` files
 * and creates the FreeRTOS tasks for network operations, sensor readings,
 * and actuator control.
 */
void setup() {
    Serial.begin(115200);

    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);

    Serial.println("Setup");
    // Sensors setup
    setColor(0, 0, 255); // Blue: setup in progress
    size_t maxCount = std::max(dhtCount, hmdCount);

    for (size_t i = 0; i < maxCount; i++) {
        // Setup DHT sensors
        if (i < dhtCount) {
            while (!DHTSensor::getInstance(i)->setupSensor()) {
                Serial.printf("Retrying DHT Sensor %d setup...\n", i + 1);
                vTaskDelay(500 / portTICK_PERIOD_MS);
            }
        }
        // Setup HMD sensors
        if (i < hmdCount) {
            while (!HMDSensor::getInstance(i)->setupSensor()) {
                Serial.printf("Retrying HMD Sensor %d setup...\n", i + 1);
                vTaskDelay(500 / portTICK_PERIOD_MS);
            }
        }
    }
    Serial.println("Sensor setup complete");
    Serial.println("Motors setup");

    // Motors setup
    maxCount = std::max(fanCount, pumpCount);
    for (size_t i = 0; i < maxCount; i++) {
        if (i < fanCount) {
            while (!Fan::getInstance(i)->setupMotor()) {
                Serial.printf("Retrying Fan %d setup...\n", i + 1);
                vTaskDelay(500 / portTICK_PERIOD_MS);
            }
        }
        if (i < pumpCount) {
            while (!Pump::getInstance(i)->setupMotor()) {
                Serial.printf("Retrying Pump %d setup...\n", i + 1);
                vTaskDelay(500 / portTICK_PERIOD_MS);
            }
        }
    }

    // Servo setup
    for (size_t i = 0; i < servoCount; i++) {
        while (!WindowMotor::getInstance(i)->setupMotor()) {
            Serial.printf("Retrying servo %d setup...\n", i + 1);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }
    Serial.println("Motor setup complete");

    // Create queues
    msg_dht_queue = xQueueCreate(msg_dht_queue_len, sizeof(TemperatureData));
    msg_hmd_queue = xQueueCreate(msg_hmd_queue_len, sizeof(MoistureData));

    if (xTaskCreatePinnedToCore(network_task, "Network task", 8192, NULL, 1, &network_handle, pro_cpu) != pdTRUE) {
        Serial.println("Failed to create task");
    }

    if (xTaskCreatePinnedToCore(read_sensors, "Read sensors task", 4096, NULL, 2, &read_sensors_handle, app_cpu) != pdTRUE) {
        Serial.println("Failed to create task");
    }

    if (xTaskCreatePinnedToCore(control_actuators, "Control actuators task", 4096, NULL, 1, &control_actuators_handle, app_cpu) != pdTRUE) {
        Serial.println("Failed to create task");
    }

    setColor(0, 255, 0); // Green: setup complete
}

/**
 * @brief Main loop function (FreeRTOS idle task).
 *
 * This function is not used in this implementation as tasks are handled by FreeRTOS.
 * It includes a delay to prevent the loop from running too frequently.
 */
void loop() {
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
