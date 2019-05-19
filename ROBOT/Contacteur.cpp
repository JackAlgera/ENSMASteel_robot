#include "Contacteur.h"

Contacteur::Contacteur(uint8_t pin)
{
  this->pin=pin;
  pinMode(pin,INPUT_PULLDOWN);
  pressed=false;
}

Contacteur::Contacteur()
{
  
}



void Contacteur::actuate()
{
    pressed=(digitalRead(pin)==HIGH);
}

bool Contacteur::isPressed(){return pressed;}
