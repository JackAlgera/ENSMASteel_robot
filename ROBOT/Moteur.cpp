#include "Moteur.h"
#include "Arduino.h"

void Motor::actuate()
{
    bypass=true;
    masterOrder=-MAXPWM;
    if (bypass)
    {
        order=masterOrder;
    }
    int orderComp=constrain((int)(order/health),-MAXPWM,MAXPWM);
    if (orderComp>0)
    {
        digitalWrite(pinSens,HIGH);
        digitalWrite(pinBrake,LOW);
        analogWrite(pinPWR,orderComp);
    }
    else if (orderComp<-0)
    {
        digitalWrite(pinSens,LOW);
        digitalWrite(pinBrake,LOW);
        analogWrite(pinPWR,abs(orderComp));
    }
    else
    {
        digitalWrite(pinSens,HIGH);
        digitalWrite(pinBrake,HIGH);
        analogWrite(pinPWR,order);
    }
}

Motor init_motor(uint8_t in_pinPWR, uint8_t in_pinSens, uint8_t in_pinBrake,float health)
{

    Motor out;
    analogWriteResolution(16);
    analogWriteFrequency(in_pinPWR,3*FREQUENCY);
    out.bypass=false;
    out.pinPWR=in_pinPWR;
    pinMode(out.pinPWR,OUTPUT);
    out.pinSens=in_pinSens;
    pinMode(out.pinSens,OUTPUT);
    out.pinBrake=in_pinBrake;
    pinMode(out.pinBrake,OUTPUT);
    out.order=0;
    out.masterOrder=0;
    out.health=health;
    return out;
}
