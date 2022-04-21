#pragma once

typedef unsigned int (*GetCounter)();
typedef bool (*ResetStepCounter)();

typedef struct 
{
    GetCounter getCounter;    
    ResetStepCounter resetStepCounter;
} BmaApi;

BmaApi watchBmaApi();