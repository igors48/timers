#pragma once

// no watch or freertos specific here. we should be able to use it in the native env

typedef void (*VoidFunc)();

typedef bool (*BoolFunc)();

typedef struct
{
    VoidFunc readIRQ;
    BoolFunc isPEKShortPressIRQ;
    VoidFunc clearIRQ;
} PowerApi;

// took watch from package name
PowerApi watchPowerApi();