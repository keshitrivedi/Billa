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
#include <Audio.h>

#include <filemgmt.h>
#include <screenState.h>
#include <animations.h>
#include <controls.h>
#include <ui.h>

#define SD_CS 15

SPIClass spi(VSPI);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define TOUCHPIN 4
#define MODE 5 //dflt
#define UP 19
#define DOWN 26
#define PAUSE 23
#define SELECT 25

#define WSEL 18
#define DIN 33
#define BCK 32

std::vector<std::string> playlistList;
std::vector<std::string>currentSongList;

ScreenState currentScreen = PLAYLIST_VIEW;

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Audio audio;

int currentAnim = 0;
int currentPlaylist = 0;
int currentSong = 0;

int prevModeState = HIGH;

// updown state change detection for oled displaying purposes acchhoo
int oledID = 0;
int prevOledID = -1;

bool songStarted = false;
bool enteringPlaylist = false;

void SDSetup() {
  spi.begin(14, 27, 13, SD_CS);

  if (!SD.begin(SD_CS, spi)) {
    Serial.println("SD Card mount failed ourr naurrrrr");
    return;
  }

  Serial.println("SD Card ready yey");

  Serial.println(ESP.getFreeHeap());
}

void setup() {
  Serial.begin(115200);

  currentPlaylist = 0;
  currentSong = 0;

  pinMode(TOUCHPIN, INPUT);
  pinMode(MODE, INPUT_PULLUP);

  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(SELECT, INPUT_PULLUP);
  pinMode(PAUSE, INPUT_PULLUP);

  SDSetup();

  audio.setBufsize(4096, 8192);

  audio.setPinout(BCK, WSEL, DIN);
  audio.setVolume(15);

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

  prevModeState = HIGH;
}

void loop() {

  if (millis() % 5000 == 0) {
    Serial.println(ESP.getFreeHeap());
  }

  int touchval = digitalRead(TOUCHPIN);
  int modeButtonState = digitalRead(MODE);
  int selectBtnState = digitalRead(SELECT);

  int upBtnState = digitalRead(UP);
  int downBtnState = digitalRead(DOWN);

  int pauseBtnState = digitalRead(PAUSE);

  bool modePressed = (prevModeState == HIGH && modeButtonState == LOW);
  prevModeState = modeButtonState;

  if (modePressed) {
    currentScreen = MODE_MENU;
    oledID = 0;
    prevOledID = -1;
  }

  pausePlay(audio, pauseBtnState);

  if (currentScreen == MODE_MENU) {
    oledID = Navigayte(modeMenu, upBtnState, downBtnState, oledID);

    std::string selected = Selectuh(modeMenu, selectBtnState, oledID);

    if (selected != "") {

        if (oledID == 0) currentScreen = ANIMATION_VIEW;
        else if (oledID == 1) {
          currentScreen = PLAYLIST_VIEW;
          currentPlaylist = 0;
          oledID = 0;
          prevOledID = -1;
          enteringPlaylist = true;
          Selectuh(modeMenu, HIGH, 0);
          Selectuh(playlistList, HIGH, 0);
          Navigayte(playlistList, HIGH, HIGH, 0);
          return;
        }
        else if (oledID == 2) currentScreen = NOW_PLAYING;

        prevOledID = -1;
    }

    if (oledID != prevOledID) {
        displayListOLED(display, modeMenu, oledID);
        prevOledID = oledID;
    }
  }

  if (currentScreen == ANIMATION_VIEW) {
    updateAnimationIndex(upBtnState, downBtnState, currentAnim, 3);

    if (touchval == 1) {
      toggle_frames_millis(uwu_frames, 700, 2, display);
      // delay(200);
    } else {
        toggle_frames_millis(
        animations[currentAnim],
        700,
        frameCounts[currentAnim],
        display
    );
    }
  }


  if (currentScreen == PLAYLIST_VIEW) {
    // listDir(SD, "/", 0, playlistList);
    // displayListOLED(display, playlistList, oledID);
    if (enteringPlaylist) {
      displayListOLED(display, playlistList, currentPlaylist);
      prevOledID = currentPlaylist;
      oledID = currentPlaylist;
      enteringPlaylist = false;
    }

    Serial.print("PLAYLIST_VIEW oledID: ");
    Serial.print(oledID);
    Serial.print(" prevOledID: ");
    Serial.println(prevOledID);

    currentPlaylist = Navigayte(playlistList, upBtnState, downBtnState, currentPlaylist);
    oledID = currentPlaylist;

    std::string selectedPlaylist = Selectuh(playlistList, selectBtnState, currentPlaylist);

    if (selectedPlaylist != "") {
      Serial.println("Selected Playlist: ");
      Serial.println(selectedPlaylist.c_str());

      currentSong = 0;
      currentScreen = SONG_VIEW;
      displayPlaylist(SD, currentPlaylist, currentSongList, playlistList);
      prevOledID = -1;
    }

    if (oledID != prevOledID) {
      displayListOLED(display, playlistList, oledID);
      prevOledID = oledID;
    }
  }

  if (currentScreen == SONG_VIEW) {
    currentSong = Navigayte(currentSongList, upBtnState, downBtnState, currentSong);
    oledID = currentSong;

    std::string selectedSong = Selectuh(currentSongList, selectBtnState, currentSong);

    if (selectedSong != "") {
      Serial.println("Selected song: ");
      Serial.println(selectedSong.c_str());

      songStarted = false;
      currentScreen = NOW_PLAYING;
    }

    if (oledID != prevOledID) {
      displayListOLED(display, currentSongList, oledID);
      prevOledID = oledID;
    }
  }

  if (currentScreen == NOW_PLAYING) {
    if(!songStarted) {
      audio.connecttoFS(SD, currentSongList[currentSong].c_str());
      songStarted = true;
    }
  }

  audio.loop();
}