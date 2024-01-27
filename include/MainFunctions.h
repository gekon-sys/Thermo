#pragma once
#include "Config.h"

void run();

float readCheckDhtData();
float readCheckHmdData();
void controlFanWindow(float &currentTemp);
void controlPump(float &currentMoist);

void connectToWifi();
void reconnect();