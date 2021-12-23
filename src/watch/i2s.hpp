#pragma once

typedef signed int (*Write)(signed int i2sNum, const void *src, signed int size, signed int *bytesWritten, unsigned int waitInMillis);

typedef struct 
{
    Write write;
} I2sApi;

I2sApi watchI2sApi();