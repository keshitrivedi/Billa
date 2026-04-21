#include <screenState.h>
#include <Arduino.h>

std::vector<std::string> modeMenu = {
    "Animations",
    "Playlists",
    "Now Playing"
};

void resetNavState(int &prevUp, int &prevDown) {
    prevUp = HIGH;
    prevDown = HIGH;
}