#pragma once
#include "configParser.h"

struct Config {
    bool zoomBool;
    bool discordRPCBool;
    int zoomKey;

};

Config getConfig(Config configObj);
int str2int(const std::string& str);
