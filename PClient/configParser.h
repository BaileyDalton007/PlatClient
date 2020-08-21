#pragma once
#include "configParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


struct Config {
    bool zoomBool;
    bool discordRPCBool;
    int zoomKey;
    std::string ytAPIkey;
    std::string ytChannelId;
    bool showKeystrokes;
    bool showCPS;
    int keyStrokesX;
    int keyStrokesY;
    int displayScale;
};

Config getConfig(Config configObj);
