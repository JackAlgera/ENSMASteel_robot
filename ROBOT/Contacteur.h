#ifndef CONTACTEUR_INCLUDED
#define CONTACTEUR_INCLUDED
#include "Arduino.h"

class Contacteur
{
    uint8_t pin;
    bool pressed;


public:
    void actuate();
    Contacteur(uint8_t pin);
    Contacteur();
    bool isPressed();
};


#endif
