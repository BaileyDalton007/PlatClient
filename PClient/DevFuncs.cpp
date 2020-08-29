#include "DevFuncs.h"
#include "PClient.h"
#include <iostream>
#include <string>
#include <sstream>
#include "cps.h"
using namespace std;

const char* toChar()
{
	stringstream ss;
	ss << "ppppppppppppppppppp" << getMenuStatus();
	std::string resultstr = ss.str();
	const char* cstr2 = resultstr.c_str();
    return cstr2;
}

