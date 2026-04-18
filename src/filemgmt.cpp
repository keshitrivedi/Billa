// display all playlists
// display all songs in playlist
// play song (could be here or in controls idk)

#include <FS.h>
#include <SD.h>
#include <Arduino.h>
#include <vector>

void listDir(fs::FS &fs, const char * dirname, uint8_t levels, int &id, std::vector<std::string>&playlistList){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());

            Serial.print("ID: ");
            Serial.println(id);

            playlistList.push_back(file.path());

            id++;
        }
        file = root.openNextFile();
    }
}

void displayPlaylist(fs::FS &fs, int id, std::vector<std::string>&currentSongList, std::vector<std::string>&playlistList) {
  currentSongList.clear();

    File currentPlaylist = fs.open(playlistList[id].c_str());

    if (!currentPlaylist || !currentPlaylist.isDirectory()) {
        Serial.println("This playlist does not exist");
        return;
    }

    File file = currentPlaylist.openNextFile();

    int song = 0;
    while (file) {
        if (!file.isDirectory()) {
            Serial.print(song);
            Serial.print(": ");
            Serial.println(file.name());
            
            currentSongList.push_back(file.path());
            song++;
        }
        file = currentPlaylist.openNextFile();
    }
}