#include "Contacteur.h"

Contacteur::Contacteur(uint8_t pin)
{
  this->pin=pin;
  this->wasPressed=false;
  pinMode(pin,INPUT_PULLDOWN);
}

Contacteur::Contacteur()
{
}

bool Contacteur::isPressed()
{
	bool value=digitalRead(pin);
  wasPressed=value;
	return value;

}

bool Contacteur::isJustPressed()
{
  bool value=digitalRead(pin);
  bool out=value && !wasPressed;
  wasPressed=value;
  return out;

}
