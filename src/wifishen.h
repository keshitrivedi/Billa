#pragma once
#include <WiFi.h>

extern WiFiServer server;

void wifi_setup(const char* ssid, const char* password);
void wifi_loop();