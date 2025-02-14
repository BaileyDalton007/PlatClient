#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "configParserOverlay.h"

using namespace std;

void loadConfig(Config& config) {
    ifstream fin("config.txt");
    string line;
    std::string a;
    std::string c;

    while (getline(fin, line)) {
        istringstream sin(line.substr(line.find("=") + 1));
        if (line.find("youtubeAPIkey") != -1)
        {
            sin >> config.ytAPIkey;
        }
        else if (line.find("youtubeChannelID") != -1)
        {
            sin >> config.ytChannelId;
        }
        else if (line.find("showKeyStrokes") != -1)
        {
            sin >> config.showKeystrokes;
        }
        else if (line.find("showCPS") != -1)
        {
            sin >> config.showCPS;
        }
        else if (line.find("keyStrokeX") != -1)
        {
            sin >> a;
            int b;
            try
            {
                b = stoi(a, 0, 10);
            }
            catch (...) {
                b = 200;
            }
            config.keyStrokesX = b;

        }
        else if (line.find("keyStrokeY") != -1)
        {
            sin >> c;
            int d;
            try
            {
                d = stoi(c, 0, 10);
            }
            catch (...) {
                d = 200;
            }
            config.keyStrokesY = d;
        }
        else if (line.find("displayScale") != -1)
        {
            sin >> a;
            int b;
            try
            {
                b = stoi(a, 0, 10);
            }
            catch (...) {
                b = 200;
            }
            config.displayScale = b;

        }
     
    }
}

Config getConfig(Config configObj) {
    loadConfig(configObj);
    return configObj;
}