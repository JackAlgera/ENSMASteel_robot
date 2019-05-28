#include "Codeuse.h"

void Codeuse::actuate(float dt)
{
    ticks=enc->read();
    deltaAvance=(ticks-oldTicks)*(PI*diametreRoue)/TICKS_PER_ROUND;  //Simple géométrie
    v=deltaAvance/dt;
    debug+=deltaAvance;
    oldTicks=ticks;
}

Codeuse::Codeuse(uint8_t pinA,uint8_t pinB,float diametreRoue)
{
    enc=new Encoder(pinA,pinB);
    this->diametreRoue=diametreRoue;
    ticks=0;
    oldTicks=0;
    v=0.0;
    deltaAvance=0;
    debug=0;
}

Codeuse::Codeuse()
{
}
