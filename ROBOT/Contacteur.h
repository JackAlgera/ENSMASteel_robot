#ifndef CONTACTEUR_INCLUDED
#define CONTACTEUR_INCLUDED
#include "Arduino.h"

class Contacteur
{
  uint8_t pin;
  bool wasPressed;
  float tStartPressed;


  public:
	  void actuate();
	  Contacteur(uint8_t pin);
	  Contacteur();
	  bool isPressed();
   bool isJustPressed();
   bool isPressedForMoreThan(float t);

};


#endif
