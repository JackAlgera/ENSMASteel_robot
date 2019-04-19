#include "Codeuse.h"
#include "CONSTANT.h"

volatile int bufferTicksGauche=0;  //Permet de stocker les ticks reçu lors des interruptions
volatile int bufferTicksDroite=0;  //idem

void Codeuse::actuate(float dt)
{
  if (GD==GAUCHE) {ticks=ticks+bufferTicksGauche; bufferTicksGauche=0;}
  else {ticks=ticks+bufferTicksDroite; bufferTicksDroite=0;}
  deltaAvance=(ticks-oldTicks)*(PI*DIAMETRE_ROUE_CODEUSE)/TICKS_PER_ROUND;  //Simple géométrie
  v=deltaAvance/dt;
  oldTicks=ticks;
  
}

//Ces deux fonctions sont appelés à chaque fois qu'une codeuse génère une interruption
//Un handler est forcément sans argument, d'où la nécéssité d'avoir des buffer globaux
void handlerGauche()
{
  if (digitalRead(PIN_CODEUSE_GAUCHE_B)==digitalRead(PIN_CODEUSE_GAUCHE_A)) bufferTicksGauche++; else bufferTicksGauche--;
}

void handlerDroite()
{
  if (digitalRead(PIN_CODEUSE_DROITE_B)==digitalRead(PIN_CODEUSE_DROITE_A)) bufferTicksDroite--; else bufferTicksDroite++;
}

Codeuse init_codeuse(uint8_t in_GD)
{
  Codeuse out;
  out.GD=in_GD;
  //On dit à l'arduino que faire en cas d'une interruption
  //Les codeuses utilisées sont en quadrature
  if (out.GD==GAUCHE) {pinMode(PIN_CODEUSE_GAUCHE_B,INPUT_PULLUP);pinMode(PIN_CODEUSE_GAUCHE_A,INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(PIN_CODEUSE_GAUCHE_A),handlerGauche,CHANGE); }
  else {pinMode(PIN_CODEUSE_DROITE_B,INPUT_PULLUP);pinMode(PIN_CODEUSE_DROITE_A,INPUT_PULLUP); attachInterrupt(digitalPinToInterrupt(PIN_CODEUSE_DROITE_A),handlerDroite,CHANGE);}
  out.ticks=0;
  out.oldTicks=0;
  out.v=0.0;
  return out;
}
