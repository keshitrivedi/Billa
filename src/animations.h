#pragma once

#include <Arduino.h>
#include <Adafruit_SH110X.h>

void toggle_frames_millis(const unsigned char* frames[], int dly, int frameNum, Adafruit_SH1106G &display);
void updateAnimationIndex(int upState, int downState, int &currentAnim, int totalAnims);

extern const unsigned char* blink_frames[2];
extern const unsigned char* search_frames[5];
extern const unsigned char* uwu_frames[2];
extern const unsigned char* uwu_clueless_frames[2];

extern const unsigned char** animations[];
extern int frameCounts[];