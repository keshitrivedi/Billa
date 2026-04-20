#pragma once

#include <Arduino.h>
#include <Adafruit_SH110X.h>
#include <vector>

void displayListOLED(Adafruit_SH1106G &display, std::vector<std::string>& list, int selectedIndex);