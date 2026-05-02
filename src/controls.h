#pragma once

#include <vector>
#include <string>
#include <Audio.h>

int Navigayte(const std::vector<std::string>&musicEntity, int upBtnState, int downBtnState, int id);
std::string Selectuh(const std::vector<std::string>&musicEntity, int selectBtnState, int id);
void pausePlay(Audio &audio, int pauseBtnState);