#pragma once

typedef unsigned int (*GetCounter)(); // todo remove typedefs
typedef bool (*ResetStepCounter)();

typedef struct 
{
    GetCounter getCounter;    
    ResetStepCounter resetStepCounter;
} BmaApi;

BmaApi watchBmaApi();