#ifndef FILTRE_INCLUDED
#define FILTRE_INCLUDED
#include "Arduino.h"
class Filtre
{
public:
    float lastValue,lastLastValue,raw,lastRaw;
    uint8_t order=1;
    float Wc;
    void in(float newRaw,float dt);
    float out();
    void reset(float value);
};

Filtre newFiltre(float initValue,float frequency,uint8_t order);
#endif
