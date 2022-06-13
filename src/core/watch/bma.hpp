#pragma once

typedef unsigned int (*GetCounter)(); // todo remove typedefs #90
typedef bool (*ResetStepCounter)();

typedef struct 
{
    GetCounter getCounter;    
    ResetStepCounter resetStepCounter;
} BmaApi;

BmaApi watchBmaApi();