#pragma once
#include <exception>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "nlohmann/json.hpp"
#include "pointerParser.h"

#include <iostream>

namespace pt = boost::property_tree;

struct Pointer {

	int fovBase;
	std::vector<unsigned int> fovOffsets;

	int handBase;
	std::vector<unsigned int> handOffsets;

	int ignBase;
	std::vector<unsigned int> ignOffsets;
};

Pointer getPointers(Pointer ptrobj);