#pragma once

#include <vector>
#include <string>

enum ScreenState {
    PLAYLIST_VIEW,
    SONG_VIEW,
    NOW_PLAYING,
    ANIMATION_VIEW,
    MODE_MENU
};

extern ScreenState currentScreen;

extern std::vector<std::string> modeMenu;