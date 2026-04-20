#include <Arduino.h>
#include <FS.h>
#include <vector>
#include "khikhi.h"
#include <WiFi.h>
#include <splash.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <SPI.h>
#include <SD.h>

#include <filemgmt.h>
#include <animations.h>
#include <controls.h>
#include <ui.h>

#define SD_CS 15

SPIClass spi(VSPI);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define TOUCHPIN 4
#define DEFAULT_BUTTON 5
#define SEARCH_BUTTON 18
#define UP 19
#define DOWN 26
#define BUZZERPIN 23
#define SELECT 25

std::vector<std::string> playlistList;
std::vector<std::string>currentSongList;

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int currentAnim = 0;
int currentPlaylist = 0;

// updown state change detection for oled displaying purposes acchhoo
int oledID = 0;
int prevOledID = -1;

void SDSetup() {
  spi.begin(14, 27, 13, SD_CS);

  if (!SD.begin(SD_CS, spi)) {
    Serial.println("SD Card mount failed ourr naurrrrr");
    return;
  }

  Serial.println("SD Card ready yey");
}

void setup() {
  Serial.begin(115200);

  currentPlaylist = 0;

  pinMode(TOUCHPIN, INPUT);
  pinMode(SEARCH_BUTTON, INPUT_PULLUP);
  pinMode(DEFAULT_BUTTON, INPUT_PULLUP);

  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(SELECT, INPUT_PULLUP);

  SDSetup();

  playlistList.clear();
  listDir(SD, "/", 0, playlistList);

  if(!display.begin(0x3C, true)) {
    Serial.println("SH1106 allocation failed");
    for(;;);
  }

  display.clearDisplay();
  oledID = 0;
  prevOledID = -1;
  displayListOLED(display, playlistList, oledID);
  prevOledID = oledID;
}

void loop() {
  int touchval = digitalRead(TOUCHPIN);
  int searchButtonState = digitalRead(SEARCH_BUTTON);
  int defaultButtonState = digitalRead(DEFAULT_BUTTON);
  int selectBtnState = digitalRead(SELECT);

  int upBtnState = digitalRead(UP);
  int downBtnState = digitalRead(DOWN);

  // if (searchButtonState == LOW) {
  //   currentAnim = 2;
  //   // Serial.println("serach");
  // }
  // if (defaultButtonState == LOW) {
  //   currentAnim = 0;
  //   // Serial.println("default");
  // }

  // if (touchval == 1) {
  //   toggle_frames_millis(uwu_frames, 700, 2, display);
  //   delay(200);
  // } else {
  //   if (currentAnim == 0) {
  //     toggle_frames_millis(blink_frames, 700, 2, display);
  //   } else if (currentAnim == 1) {
  //     toggle_frames_millis(uwu_frames, 700, 2, display);
  //   } else if (currentAnim == 2) {
  //     toggle_frames_millis(uwu_clueless_frames, 700, 2, display);
  //   }
  // }

  currentPlaylist = Navigayte(playlistList, upBtnState, downBtnState, currentPlaylist);
  oledID = currentPlaylist;

  std::string selectedPlaylist = Selectuh(playlistList, selectBtnState, currentPlaylist);

  if (selectedPlaylist != "") {
    Serial.println("Selected Playlist: ");
    Serial.println(selectedPlaylist.c_str());

    displayPlaylist(SD, currentPlaylist, currentSongList, playlistList);
  }

  if (oledID != prevOledID) {
    displayListOLED(display, playlistList, oledID);
    prevOledID = oledID;
  }
  // listDir(SD, "/", 0, currentPlaylist, playlistList);
}