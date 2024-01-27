#include "MainFunctions.h"

void run()
{
    currentTemp = readCheckDhtData();
    currentMoist = readCheckHmdData();
    controlFanWindow(currentTemp);
    controlPump(currentMoist);
}

float readCheckDhtData()
{
    float currentTemp = dhtSensor1.read();
    while (1) // Run the loop unti the data are valid.
    {
        if (dhtSensor1.checkData(currentTemp)) // If the data are valid then exit the loop
            break;
        else
        {
            delay(1000);
            currentTemp = dhtSensor1.read(); // Read data again
        }
    }
    // Serial.println(currentTemp);
    return currentTemp;
}

float readCheckHmdData()
{
    float currentMoist = hmdSensor1.read();
    while (1)
    {
        if (hmdSensor1.checkData(currentMoist))
            break;
        else
        {
            delay(1000);
            currentMoist = hmdSensor1.read();
        }
    }
    // Serial.println(currentMoist);
    return currentMoist;
}

void controlFanWindow(float &currentTemp)
{
    if (dhtSensor1.isCloseToThreshold(currentTemp))
    {
        // Serial.println("Temperature above threshold. FAN: OPEN WINDOW: OPEN");
        fanMotor1.open();
        if (!servoMotor1.getState())
            servoMotor1.open();
    }
    else
    {
        // Serial.println("Temperature below threshold. FAN: CLOSE WINDOW: CLOSE");
        fanMotor1.close();
        if (servoMotor1.getState())
            servoMotor1.close();
    }
}

void controlPump(float &currentMoist)
{
    if (hmdSensor1.isCloseToThreshold(currentMoist))
    {
        // Serial.println("Humidity above threshold. PUMP: CLOSE");
        pumpMotor1.close();
    }
    else
    {
        // Serial.println("Humidity below threshold. PUMP: OPEN");
        pumpMotor1.open();
    }
}

void connectToWifi()
{
    WiFi.begin(SSID, PASSWORD);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < MAX_CONNECTION_ATTEMPTS)
    {
        // Serial.print(".");
        delay(300);
    }
    // Serial.print("Connected to WiFi...");
    Serial.println(WiFi.localIP());
}

void reconnect()
{
    if (WiFi.status() == WL_DISCONNECTED)
    {
        connectToWifi();
    }
}

