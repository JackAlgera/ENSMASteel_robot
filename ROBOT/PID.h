#ifndef PID_INCLUDED
#define PID_INCLUDED

#include "CONSTANT.h"
#include "Arduino.h"
#include "Fifo.h"
#include "Moteur.h"
#include "Ghost.h"

/*
En mode RUSH:
Les Goto doivent être des lignes droites.
Les Spin marchent niquel

En mode STD: (STANDARD)
Ca marche bien, les legeres courbes sont autorisées.

En mode ACRT: (ACCURATE)
Toute les courbes sont possibles. Vitesse lente. Le I est assez elevé ce qui permet de passer les obstacles

En mode DYDM: (DON'T YOU DARE MOVE)
Beaucoup de I. C'est le mode statique, verrouillé sur place
 */

class PID
{
  uint8_t PIDnervLIN=DYDM,PIDnervANG=DYDM;
  float IL=0.0,IA=0.0;
  public:
  Ghost* pointeurSurGhost;                    //Pointeur sur le fantome
  Fifo* pointeurSurFifo;                      //Pointeur sur le Fifo
  Motor* pointeurSurMoteurGauche;             //Pointeur sur le moteur gauche
  Motor* pointeurSurMoteurDroite;             //Pointeur sur le moteur droite
  void actuate(float dt,VectorE posERobot,float vRobot,float wRobot);              //Actualise les PID (compare la position du ghost et du robot, et donne les ordres au moteur en fonction
 
};

PID init_PID(Motor* in_pointeurSurMoteurGauche,Motor* in_pointeurSurMoteurDroite,Fifo* in_pointeurSurActions,Ghost* in_pointeurSurGhost);
#endif
