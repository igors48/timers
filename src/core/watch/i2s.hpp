#pragma once

typedef signed int (*Write)(const void *src, signed int size, signed int *bytesWritten); // todo remove typedef #90

typedef struct 
{
    Write write;
} I2sApi;

I2sApi watchI2sApi();