#include <Arduino.h>
#include "driver/i2s.h"
#include "i2s.hpp"

signed int i2sWrite(signed int i2sNum, const void *src, signed int size, signed int *bytesWritten, unsigned int waitInMillis)
{
    return i2s_write((i2s_port_t)i2sNum, src, size, (size_t *)bytesWritten, (TickType_t)waitInMillis / portTICK_PERIOD_MS);
}

I2sApi watchI2sApi()
{
    return {
        .write = i2sWrite
    };
}