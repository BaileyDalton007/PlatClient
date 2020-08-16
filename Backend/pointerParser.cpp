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
    int b = std::stoi(fovA, 0, 16);
    pointer.fovBase = b;



    std::string offsets[7];
    int y = 0;
    for (pt::ptree::value_type& cell : root.get_child("fov.offsets"))
    {
        offsets[y] = cell.second.get_value<std::string>();
        y++;
    }

    std::vector<unsigned int> fovOffsets{};
    for (int i = 0; i < 7; i++)
    {
        int c = std::stoi(offsets[i], 0, 16);
        fovOffsets.push_back(c);
    }
    std::reverse(fovOffsets.begin(), fovOffsets.end());
    pointer.fovOffsets = fovOffsets;

    //hand
    std::string handA = root.get<std::string>("hand.base");
    int g = std::stoi(handA, 0, 16);
    pointer.handBase = g;

    std::string handoff[7];
    int u = 0;
    for (pt::ptree::value_type& cell : root.get_child("hand.offsets"))
    {
        handoff[u] = cell.second.get_value<std::string>();
        u++;
    }

    std::vector<unsigned int> handOffsets{};
    for (int i = 0; i < 7; i++)
    {
        int c = std::stoi(handoff[i], 0, 16);
        handOffsets.push_back(c);
    }
    std::reverse(handOffsets.begin(), handOffsets.end());
    pointer.handOffsets = handOffsets;

    //menu
    std::string menuA = root.get<std::string>("menu.base");
    int p = std::stoi(menuA, 0, 16);
    pointer.menuBase = p;

    std::string menuoff[7];
    int d = 0;
    for (pt::ptree::value_type& cell : root.get_child("menu.offsets"))
    {
        menuoff[d] = cell.second.get_value<std::string>();
        d++;
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
