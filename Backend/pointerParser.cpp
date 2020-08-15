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
    pt::read_json("../pointers.json", root);  // Loads the json file in this ptree

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
    std::cout << fovOffsets.size() << std::endl;

}

Pointer getPointers(Pointer ptrobj) {
    readJson(ptrobj);
    return ptrobj;
}
