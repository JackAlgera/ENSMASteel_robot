#include "Filtre.h"
#include "Arduino.h"

#include "CONSTANT.h"
void Filtre::in(float newRaw,float dt)
{
  //ON stock la valeur brute au cas ou
  raw=newRaw;
  
  if (dt>0.5 or abs(newRaw-lastRaw)>1.5) {lastValue=newRaw; lastLastValue=newRaw; lastRaw=newRaw;} //Le filtre n'a pas ete appelle pendant trop de temps ou il y a eu une discontinuite (modulo PI) il n'a plus de sens donc on le reset
  else 
    {
      switch (order)
      {
        case 0: {lastValue=newRaw;break;} //A l'ordre 0, on ne fait rien
        case 1: {lastValue=(lastValue+newRaw*dt*Wc)/(Wc*dt+1);break;} 
        case 2:
          {
            float alpha=Wc*dt;
            float out=( alpha*newRaw + 2*lastValue*(1+1/alpha) - lastLastValue/alpha ) / (alpha + 2 + 1/alpha);
            lastLastValue=lastValue;
            lastValue=out;
            break;
          }
        case 3:
        {
          lastValue=(newRaw - Wc*(newRaw - lastRaw - lastValue)/dt ) / (1 + Wc/dt);
          lastRaw=newRaw;
        }
      }
    }
}

float Filtre::out()
{
  return lastValue; 
}

Filtre newFiltre(float initValue,float frequency,uint8_t order)
{
  Filtre out;
  out.lastValue=initValue;
  out.lastLastValue=initValue;
  out.lastRaw=initValue;
  out.order=order;
  (order==3)?(out.Wc=frequency/2.0):(out.Wc=2*PI*frequency);
  return out;
}

void Filtre::reset(float value)
{
  lastValue=value;
}
