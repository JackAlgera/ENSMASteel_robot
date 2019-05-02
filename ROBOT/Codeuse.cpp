#include "Codeuse.h"

void Codeuse::actuate(float dt)
{
    ticks=enc->read();
    deltaAvance=(ticks-oldTicks)*(PI*DIAMETRE_ROUE_CODEUSE)/TICKS_PER_ROUND;  //Simple géométrie
    v=deltaAvance/dt;
    Serial.println(v);
    oldTicks=ticks;
}

Codeuse::Codeuse(uint8_t pinA,uint8_t pinB)
{
    enc=new Encoder(pinA,pinB);
    ticks=0;
    oldTicks=0;
    v=0.0;
    deltaAvance=0;
}

Codeuse::Codeuse()
{
}
