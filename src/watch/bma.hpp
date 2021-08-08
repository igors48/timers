#pragma once

typedef unsigned int (*GetCounter)();

typedef struct 
{
    GetCounter getCounter;    
} BmaApi;

BmaApi watchBmaApi();