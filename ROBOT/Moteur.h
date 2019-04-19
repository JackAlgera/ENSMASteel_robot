#ifndef MOTEUR_INCLUDED
#define MOTEUR_INCLUDED

#include "CONSTANT.h"
#include "Arduino.h"

class Motor
{
  public:
  uint8_t pinPWR,pinSens1,pinSens2; //Pin de puissance (PWM) et les deux pin pour donner le sens de rotation
  int order,masterOrder;                        //Le regime moteur entre -255 et 255
  void actuate();                   //Actualisation de la puissance moteur
  float health=1.0;
  bool bypass=false;
  
};

Motor init_motor(uint8_t in_pinPWR, uint8_t in_pinSens1, uint8_t in_pinSens2,float health);

#endif
