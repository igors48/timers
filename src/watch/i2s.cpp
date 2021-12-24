#include <Arduino.h>
#include "driver/i2s.h"
#include "i2s.hpp"

signed int i2sWrite(const void *src, signed int size, signed int *bytesWritten)
{
    return i2s_write(I2S_NUM_0, src, size, (size_t *)bytesWritten, portMAX_DELAY);
}

I2sApi watchI2sApi()
{
    return {
        .write = i2sWrite
    };
}