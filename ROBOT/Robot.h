#ifndef ROBOT_INCLUDED
#define ROBOT_INCLUDED
#include "1_CONSTANT.h"
#include "Moteur.h"
#include "Codeuse.h"
#include "Fifo.h"
#include "Moteur.h"
#include "PID.h"
#include "Polynome.h"
#include "Vector.h"
#include "Arduino.h"
#include "Filtre.h"
#include "Comm.h"
#include "Cerveau.h"
#include "Encoder.h"
#include "Contacteur.h"

class Robot
{
public:
    float dt;
    Motor moteurGauche, moteurDroite;
    Codeuse codeuseGauche,codeuseDroite;
    Contacteur contacteurDroite,contacteurGauche;
    Ghost ghost;
    VectorE posE;                                 //Position du robot
    Filtre vF,wF;
    Fifo ordresFifo;                              //Une liste d'ordres a effectuer
    PID pid;
    Comm comm;
    Cerveau* master;

    void actuateODO(float dt);
    void set(float x0,float y0, float theta0);    //Remplie les champs de l'objet Robot
    void actuate(float dt);                       //Actualise les valeurs
    void autoTune(uint8_t nerv,uint8_t coeff,float delta,uint8_t type);
};

#endif
