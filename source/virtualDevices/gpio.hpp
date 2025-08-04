#pragma once

namespace virtualDevice
{
class virtualGPIO
{
    void setMode(bool mode);

    bool read();

    void write(bool output);
};
}