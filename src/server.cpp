#include "server.h"
#include "webpage.h"

void connectToWifi() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(SSID, PASSWORD);
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            vTaskDelay(300 / portTICK_PERIOD_MS);
        }

        Serial.print("Connected...");
        Serial.println(WiFi.localIP());
    }
}

void handleXML() {
    String xml = "<?xml version=\"1.0\"?>\n<Data>\n";
    
    // Add DHT sensor data
    for (size_t i = 0; i < DHTSensor::getCounter(); i++) {
        xml += "<Sensor>\n";
        xml += "<ID>Sensor</ID>\n";  // Added ID
        xml += "<Type>Temperature</Type>\n";  // Type added for example
        xml += "<Name>" + String(DHTSensor::getInstance(i)->getName()) + "</Name>\n";
        xml += "<Pin>" + String(DHTSensor::getInstance(i)->getPin()) + "</Pin>\n";
        xml += "<Value>" + String(temperature.tempArray[i]) + "°C</Value>\n";
        xml += "<Status>" + String((DHTSensor::getInstance(i)->getStatus()) ? "Operational" : "ERROR") + "</Status>\n";
        xml += "</Sensor>\n";
    }

    // Add HMD sensor data
    for (size_t i = 0; i < HMDSensor::getCounter(); i++) {
        xml += "<Sensor>\n";
        xml += "<ID>Sensor</ID>\n";  // Added ID
        xml += "<Type>Humidity</Type>\n";  // Type added for example
        xml += "<Name>" + String(HMDSensor::getInstance(i)->getName()) + "</Name>\n";
        xml += "<Pin>" + String(HMDSensor::getInstance(i)->getPin()) + "</Pin>\n";
        xml += "<Value>" + String(moisture.moistArray[i]) + "%</Value>\n";
        xml += "<Status>" + String((HMDSensor::getInstance(i)->getStatus()) ? "Operational" : "ERROR") + "</Status>\n";
        xml += "</Sensor>\n";
    }

    // Add Fan data
    for (size_t i = 0; i < Fan::getCounter(); i++) {
        xml += "<Motor>\n";
        xml += "<ID>Motor</ID>\n";  // Added ID
        xml += "<Type>12V DC</Type>\n";  // Type added for example
        xml += "<Name>" + String(Fan::getInstance(i)->getName()) + "</Name>\n";
        xml += "<Pin>" + String(Fan::getInstance(i)->getPin()) + "</Pin>\n";
        xml += "<Status>" + String(Fan::getInstance(i)->getState() ? "On" : "Off") + "</Status>\n";
        xml += "</Motor>\n";
    }

    // Add Pump data
    for (size_t i = 0; i < Pump::getCounter(); i++) {
        xml += "<Motor>\n";
        xml += "<ID>Motor</ID>\n";  // Added ID
        xml += "<Type>12V DC</Type>\n";  // Type added for example
        xml += "<Name>" + String(Pump::getInstance(i)->getName()) + "</Name>\n";
        xml += "<Pin>" + String(Pump::getInstance(i)->getPin()) + "</Pin>\n";
        xml += "<Status>" + String(Pump::getInstance(i)->getState() ? "On" : "Off") + "</Status>\n";
        xml += "</Motor>\n";
    }

    // Add WindowMotor data
    for (size_t i = 0; i < WindowMotor::getCounter(); i++) {
        xml += "<Motor>\n";
        xml += "<ID>Motor</ID>\n";  // Added ID
        xml += "<Type>5V Servo</Type>\n";  // Type added for example
        xml += "<Name>" + String(WindowMotor::getInstance(i)->getName()) + "</Name>\n";
        xml += "<Pin>" + String(WindowMotor::getInstance(i)->getPin()) + "</Pin>\n";
        xml += "<Status>" + String(WindowMotor::getInstance(i)->getState() ? "Open" : "Closed") + "</Status>\n";
        xml += "</Motor>\n";
    }

    xml += "</Data>\n";
    server.send(200, "text/xml", xml);
}



void handleRoot() {
    Serial.println("Sending web page");
    server.send_P(200, "text/html", MAIN_PAGE);
}

void runServer() {
    server.on("/", handleRoot);
    server.on("/xml", handleXML);
    server.begin();
}
