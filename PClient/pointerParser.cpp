#include <exception>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "nlohmann/json.hpp"
#include "pointerParser.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include <iostream>

namespace pt = boost::property_tree;

pt::ptree root;      // Creates a root

void readJson(Pointer& pointer) {
    pt::read_json("pointers.json", root);  // Loads the json file in this ptree


    //fov 
    std::string fovA = root.get<std::string>("fov.base");
    int fovB = std::stoi(fovA, 0, 16);
    pointer.fovBase = fovB;



    std::string fovPoffsets[7];
    int fovY = 0;
    for (pt::ptree::value_type& cell : root.get_child("fov.offsets"))
    {
        fovPoffsets[fovY] = cell.second.get_value<std::string>();
        fovY++;
    }

    std::vector<unsigned int> fovOffsets{};
    for (int i = 0; i < 7; i++)
    {
        int c = std::stoi(fovPoffsets[i], 0, 16);
        fovOffsets.push_back(c);
    }
    std::reverse(fovOffsets.begin(), fovOffsets.end());
    pointer.fovOffsets = fovOffsets;

    //hand
    std::string handA = root.get<std::string>("hand.base");
    int handB = std::stoi(handA, 0, 16);
    pointer.handBase = handB;

    std::string handoff[7];
    int handY = 0;
    for (pt::ptree::value_type& cell : root.get_child("hand.offsets"))
    {
        handoff[handY] = cell.second.get_value<std::string>();
        handY++;
    }

    std::vector<unsigned int> handOffsets{};
    for (int i = 0; i < 7; i++)
    {
        int c = std::stoi(handoff[i], 0, 16);
        handOffsets.push_back(c);
    }
    std::reverse(handOffsets.begin(), handOffsets.end());
    pointer.handOffsets = handOffsets;

    //ign
    std::string ignA = root.get<std::string>("ign.base");
    int ignB = std::stoi(ignA, 0, 16);
    pointer.ignBase = ignB;



    std::string ignoffsets[7];
    int ignY = 0;
    for (pt::ptree::value_type& cell : root.get_child("ign.offsets"))
    {
        ignoffsets[ignY] = cell.second.get_value<std::string>();
        ignY++;
    }

    std::vector<unsigned int> ignOffsets{};
    for (int i = 0; i < 7; i++)
    {
        int c = std::stoi(ignoffsets[i], 0, 16);
        ignOffsets.push_back(c);
    }
    std::reverse(ignOffsets.begin(), ignOffsets.end());
    pointer.ignOffsets = ignOffsets;

    //menu
    std::string menuA = root.get<std::string>("menu.base");
    int menuB = std::stoi(menuA, 0, 16);
    pointer.menuBase = menuB;

    std::string menuoff[7];
    int menuY = 0;
    for (pt::ptree::value_type& cell : root.get_child("menu.offsets"))
    {
        menuoff[menuY] = cell.second.get_value<std::string>();
        menuY++;
    }

    std::vector<unsigned int> menuOffsets{};
    for (int i = 0; i < 7; i++)
    {
        int c = std::stoi(menuoff[i], 0, 16);
        menuOffsets.push_back(c);
    }
    std::reverse(menuOffsets.begin(), menuOffsets.end());
    pointer.menuOffsets = menuOffsets;
}

Pointer getPointers(Pointer ptrobj) {
    readJson(ptrobj);
    return ptrobj;
}
