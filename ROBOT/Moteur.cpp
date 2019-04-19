#include "Moteur.h"
#include "Arduino.h"
#include "TimerOne.h"

void Motor::actuate()
{
  if (bypass) order=masterOrder;
  int orderComp=constrain((int)(order/health),-MAXPWM,MAXPWM);
  if (orderComp>0) {digitalWrite(pinSens1,HIGH); digitalWrite(pinSens2,LOW);Timer1.setPwmDuty(pinPWR,orderComp);}
  else if (orderComp<-0) {digitalWrite(pinSens1,LOW); digitalWrite(pinSens2,HIGH);Timer1.setPwmDuty(pinPWR,abs(orderComp));}
  else {digitalWrite(pinSens1,HIGH); digitalWrite(pinSens2,LOW);Timer1.setPwmDuty(pinPWR,order);}
}

Motor init_motor(uint8_t in_pinPWR, uint8_t in_pinSens1, uint8_t in_pinSens2,float health)
{
  Timer1.pwm(in_pinPWR,0,(uint32_t) (0.5/FREQUENCY*1000000.0));
  Motor out;
  out.bypass=false;
  out.pinPWR=in_pinPWR;
  out.pinSens1=in_pinSens1;
  pinMode(out.pinSens1,OUTPUT);
  out.pinSens2=in_pinSens2;
  pinMode(out.pinSens2,OUTPUT);
  out.order=0;
  out.masterOrder=0;
  out.health=health;
  return out;
}
