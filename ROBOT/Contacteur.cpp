#include "Contacteur.h"

Contacteur::Contacteur(uint8_t pin)
{
  this->pin=pin;
  this->wasPressed=false;
  pinMode(pin,INPUT_PULLDOWN);
  tStartPressed=millis()/1000.0;
}

Contacteur::Contacteur()
{
}

void Contacteur::actuate()
{
    isPressed();
}

bool Contacteur::isPressed()
{
	bool value=digitalRead(pin);
	if (value==HIGH && wasPressed==LOW)
        tStartPressed=millis()/1000.0;
    if (value==LOW)
        tStartPressed=millis()/1000.0;
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

bool Contacteur::isPressedForMoreThan(float t)
{
    if (isPressed() && (millis()/1000.0-tStartPressed)>t)
        return true;
    else
        return false;
}
