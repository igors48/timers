#pragma once

typedef void (*Buzz)(int duration);

typedef struct {
    Buzz buzz;    
} MotorApi;

MotorApi watchMotorApi();
