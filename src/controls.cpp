// Navigation (up and down)
// Animation Changer
// Selection
// mode
// go back
#include <Arduino.h>
#include <vector>
#include <string>

#include <controls.h>

int Navigayte (const std::vector<std::string>&musicEntity, int upBtnState, int downBtnState, int id) {

    // handle this later pls
    if (musicEntity.empty()) {
        return -1;
    }

    static int prevUpState = HIGH;
    static int prevDownState = HIGH;

    if (prevUpState == HIGH && upBtnState == LOW) {
        if (id < musicEntity.size() - 1) {
            id++;
        }
    }

    if (prevDownState == HIGH && downBtnState == LOW) {
        if (id > 0) {
            id--;
        }
    }

    if (id < 0) {id = 0;}
    if (id >= musicEntity.size()) {id = musicEntity.size() - 1;}

    prevUpState = upBtnState;
    prevDownState = downBtnState;

    Serial.println(id);
    Serial.println(musicEntity[id].c_str());
    return id;
}

std::string Selectuh (const std::vector<std::string>&musicEntity, int selectBtnState, int id) {

    if (musicEntity.empty()) {
        return "";
    }

    static int prevSelectState = HIGH;

    std::string result = "";    

    if (selectBtnState == LOW && prevSelectState == HIGH && id >= 0 && id < musicEntity.size()) {
        Serial.println(musicEntity[id].c_str());
        result = musicEntity[id];
    }

    prevSelectState = selectBtnState;

    return result;
}