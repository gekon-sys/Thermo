#include "Config.h"
#include "MainFunctions.h"
#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int ldrData = 0;
void setup()
{
  Serial.begin(115200);
  connectToWifi();

  // Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    // Serial.println("signup...OK");
    signupOK = true;
  }
  else
    Serial.printf("%s\n", config.signer.signupError.message.c_str());

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);

  dhtSensor1.setupDHT();
  hmdSensor1.setupHMD();
  fanMotor1.setupMotor();
  pumpMotor1.setupMotor();
  servoMotor1.setupMotor();
}

void loop()
{

  reconnect();
  run();

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    char dhtDataToString[8];
    char hmdDataToString[8];

    dtostrf(currentTemp, 6, 1, dhtDataToString);
    dtostrf(currentMoist, 6, 1, hmdDataToString);
    if (Firebase.setString(fbdo, F("Data/Temperature"), dhtDataToString) && Firebase.setString(fbdo, F("Data/Humidity"), hmdDataToString))
      Serial.println("Float value set successfully");
    else
    {
      // Serial.println("Failed to set float value");
      Serial.printf("Reason: %s\n", fbdo.errorReason());
    }
    char retrievedDhtDataString[8];
    char retrievedHmdDataString[8];
    Firebase.getString(fbdo, F("Data/Temperature"), retrievedDhtDataString);
    Firebase.getString(fbdo, F("Data/Humidity"), retrievedHmdDataString);

    Firebase.setBool(fbdo, F("States/Pump_State"), pumpMotor1.getState());
    Firebase.setBool(fbdo, F("States/Fan_State"), fanMotor1.getState());
    Firebase.setBool(fbdo, F("States/Servo_State"), servoMotor1.getState());

    // float retrievedDhtData = atof(retrievedDhtDataString);
    // float retrievedHmdData = atof(retrievedHmdDataString);
  }
  delay(2000);
}
