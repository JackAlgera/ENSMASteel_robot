#ifndef CODEUSE_INCLUDED
#define CODEUSE_INCLUDED

#include "1_CONSTANT.h"
#include "Arduino.h"

class Codeuse
{
  public:
  uint8_t GD;                         //Codeuse gauche ou droite
  int32_t ticks=0,oldTicks=0;         //Nombre de ticks compté, nombre de ticks compté au dernier appel
  float v=0.0,deltaAvance=0.0;        //Vitesse et avance du robot AU NIVEAU DE LA ROUE CODEUSE
  void actuate(float dt);             //Actualise (transforme les ticks en vitesse puis en avance)
};

Codeuse init_codeuse(uint8_t in_GD);
#endif
