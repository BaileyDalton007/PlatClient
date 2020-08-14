#pragma once
#include "configParserOverlay.h"
#include <string>
#include <iostream>

struct Config {
    std::string ytAPIkey;
    std::string ytChannelId;
    bool showKeystrokes;
    bool showCPS;
    int keyStrokesX;
    int keyStrokesY;
    int displayScale;
};

Config getConfig(Config configObj);
