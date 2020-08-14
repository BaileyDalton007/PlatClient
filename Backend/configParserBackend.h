#pragma once
#include "configParserBackend.h"

struct Config {
    bool zoomBool;
    bool discordRPCBool;
    int zoomKey;
    int displayScale;
};

Config getConfig(Config configObj);
int str2int(const std::string& str);
