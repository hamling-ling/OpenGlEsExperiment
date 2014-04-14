#include "stdafx.h"
#include "modeltool.h"
#include <cstring>
#include <algorithm>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

namespace osakanaengine 
{
    bool StringEndsWith(std::string const &fullString, std::string const &ending)
    {
        if (fullString.length() >= ending.length()) {
            return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
        } else {
            return false;
        }
    }
}