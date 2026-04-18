#pragma once

#include <FS.h>
#include <SD.h>
#include <Arduino.h>
#include <vector>

void listDir(fs::FS &fs, const char * dirname, uint8_t levels, std::vector<std::string>&playlistList);
void displayPlaylist(fs::FS &fs, int id, std::vector<std::string>&currentSongList, std::vector<std::string>&playlistList);