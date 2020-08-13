#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "configParser.h"

using namespace std;

int str2int(const std::string& str) {
    std::stringstream ss(str);
    int num;
    if ((ss >> num).fail())
    {
        //ERROR 
    }
    return num;
}



void loadConfig(Config& config) {
    ifstream fin("config.txt");
    string line;
    std::string a;
    while (getline(fin, line)) {
        istringstream sin(line.substr(line.find("=") + 1));
        if (line.find("zoomEnabled") != -1)
        {
            sin >> config.zoomBool;
        }
        else if (line.find("discordPresence") != -1)
        {
            sin >> config.discordRPCBool;
        }
        else if (line.find("zoomKey") != -1)
        {
            sin >> a;
            int b = stoi(a, 0, 16);
            config.zoomKey = b;
        }

    }
}

Config getConfig(Config configObj) {
    loadConfig(configObj);
    return configObj;
}