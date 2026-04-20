#include <Arduino.h>
#include <Adafruit_SH110X.h>
#include <vector>
#include <string.h>

// displayPlaylistOLED will take the display instance and the playlist, void
// the y position of the "highlight box" will increment by a set val everytime up or down button is pressed

void displayListOLED(Adafruit_SH1106G &display, std::vector<std::string>& list, int selectedIndex) {
    if (list.empty()) {
        display.clearDisplay();
        display.setCursor(10, 20);
        display.print("No files");
        display.display();
        return;
    }

    display.clearDisplay();
    display.setTextSize(1);

    int lineHeight = 10;
    int maxVisible = 6;

    int start = selectedIndex - maxVisible / 2;
    if (start < 0) start = 0;
    if (start > list.size() - maxVisible) start = list.size() - maxVisible;
    if (start < 0) start = 0;

    for (int i = 0; i < maxVisible && (start + i) < list.size(); i++) {
        int index = start + i;
        int y = i * lineHeight;

        if (index == selectedIndex) {
            display.fillRect(0, y, 128, lineHeight, SH110X_WHITE);
            display.setTextColor(SH110X_BLACK);
        } else {
            display.setTextColor(SH110X_WHITE);
        }

        display.setCursor(2, y + 2);

        std::string name = list[index];

        if (name.length() > 16) {
            name = name.substr(0, 16) + "...";
        }

        display.print(name.c_str());
    }

    display.display();
}