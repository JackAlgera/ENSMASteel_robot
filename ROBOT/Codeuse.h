#ifndef CODEUSE_INCLUDED
#define CODEUSE_INCLUDED

#include "1_CONSTANT.h"
#include "Arduino.h"
#include "Encoder.h"

class Codeuse
{
  public:
  Codeuse(uint8_t in_GD);
  Codeuse();
  Encoder enc(0,1);
  int32_t ticks,oldTicks;     //Nombre de ticks compté, nombre de ticks compté au dernier appel
  float v,deltaAvance;        //Vitesse et avance du robot AU NIVEAU DE LA ROUE CODEUSE
  void actuate(float dt);     //Actualise (transforme les ticks en vitesse puis en avance)
};
#endif
