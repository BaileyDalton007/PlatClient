#pragma once
#include "configParserBackend.h"


struct Config {
    bool zoomBool;
    bool discordRPCBool;
    int zoomKey;
};

Config getConfig(Config configObj);
