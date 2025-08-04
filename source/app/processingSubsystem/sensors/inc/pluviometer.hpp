#pragma once

#include <cstdio>

namespace sensor
{
class pluviometer
{   
    bool init();
    virtual uint16_t getRain();
};
}