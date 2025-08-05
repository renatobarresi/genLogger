#pragma once

#include <cstdint>

namespace sensor
{
class pluviometer
{   
    virtual bool init() = 0;
    virtual uint16_t getRain() = 0;
};

class davisPluviometer: public pluviometer
{
    virtual bool init() override;
    virtual uint16_t getRain() override;
};
}