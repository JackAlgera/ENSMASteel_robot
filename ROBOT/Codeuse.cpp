#include "Codeuse.h"
#include "1_CONSTANT.h"

void Codeuse::actuate(float dt)
{
  ticks=enc.read();
  deltaAvance=(ticks-oldTicks)*(PI*DIAMETRE_ROUE_CODEUSE)/TICKS_PER_ROUND;  //Simple géométrie
  v=deltaAvance/dt;
  oldTicks=ticks;
}

Codeuse::Codeuse(uint8_t in_GD)
{
  
  //if (in_GD==GAUCHE) enc=*(new Encoder(PIN_CODEUSE_GAUCHE_B,PIN_CODEUSE_GAUCHE_A));
  //else enc=*(new Encoder(PIN_CODEUSE_DROITE_B,PIN_CODEUSE_DROITE_A));
  ticks=0;
  oldTicks=0;
  v=0.0;
  deltaAvance=0;
}

Codeuse::Codeuse()
{
  
}
