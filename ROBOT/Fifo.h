//Fifo pour FIRST IN FIRST OUT
//C'est ce fichier qui organise la succession des actions
#ifndef FIFO_INCLUDED
#define FIFO_INCLUDED

#include "CONSTANT.h"
#include "Vector.h"
#include "Arduino.h"

struct GoTo
{
  uint8_t nerv;
  float fleche;         //La flèche est sans unité. Elle indique de combien le robot peut d'éloigner du trajet le plus court
  Vector posAim;        //Position cibe
  float thetaAim;       //Deceleration, angle cible, vitesse cible
  bool arret;
};

struct Spin
{
  uint8_t nerv;
  float thetaAim;       //angle cible, acceleration et deceleration angulaire (theta point point)
};

struct FWD
{
  float acc,v;          //Acceleration, vitesse max, temps maximum passé sur cette instruction
};

struct BWD
{
  float acc,v;
};

struct STBY
{
  uint8_t nerv;
  char unlockMessage[4];
};

struct POST
{
  char message[4];
};

struct Order
{
  uint8_t type;                 //Type de l'action (CF #define)
  uint8_t timeoutDs;            //Dixième de secondes
  union                         //Une union permet de dire que uniquement l'un de ces champs à un intéret
  {
    GoTo goTo;
    Spin spin;
    FWD fwd;
    BWD bwd;
    STBY stby;
    POST post;
  };
};

class Fifo
{
  Order liste[TAILLEFIFO];
  uint8_t indiceDebut=1,indiceFin=0;
  void add(Order order);
  public:
  uint8_t inBuffer=0;
  void addGoto(uint8_t nerv,float fleche,float xAim,float yAim,float thetaAim,bool arret,uint8_t timeoutDs);  //abs(thetaAim-thetaIni)<=PI
  void addSpin(uint8_t nerv,float thetaAim,uint8_t timeoutDs);
  void addFWD(float acc,float v,uint8_t timeoutDs);
  void addBWD(float acc,float v,uint8_t timeoutDs);
  void addSTBY(uint8_t nerv,const char unlockMessage[],uint8_t timeout);
  Order* ptrFst();                     //Renvoie le pointeur vers l'action le plus ancienne (celle a effectuer normalement)
  void pop();                          //Fait sauter l'action la plus ancienne
  void clean();       //Vide le buffer et ajoute un STBY
};

Fifo init_FIFO();

#endif
