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
#include "Robot.h"
#include "ContreMesure.h"

void Robot::actuate(float dt)
{
    codeuseGauche.actuate(dt);
    codeuseDroite.actuate(dt);
    actuateODO(dt);
    ghost.actuate(dt);
    pid.actuate(dt,posE,vF.out(),wF.out());
    comm.actuate(posE,vF.out());
    master->actuate();
}

void Robot::actuateODO(float dt)
{
    vF.in((codeuseDroite.v+codeuseGauche.v)/2.0,dt);
    wF.in((codeuseDroite.v-codeuseGauche.v)/ELOIGNEMENT_CODEUSES,dt);
    posE.theta = normalize(posE.theta+(codeuseDroite.deltaAvance-codeuseGauche.deltaAvance)/ELOIGNEMENT_CODEUSES);
    float avance = (codeuseDroite.deltaAvance+codeuseGauche.deltaAvance)/2;
    posE.vec.x += avance*cos(posE.theta);
    posE.vec.y += avance*sin(posE.theta);
}


void Robot::set(float x0,float y0, float theta0)
{
    delay(1000);
    moteurDroite  = init_motor(PIN_MOTEUR_DROITE_PWR,PIN_MOTEUR_DROITE_SENS,PIN_MOTEUR_DROITE_BRAKE,1.0);
    moteurGauche  = init_motor(PIN_MOTEUR_GAUCHE_PWR,PIN_MOTEUR_GAUCHE_SENS,PIN_MOTEUR_GAUCHE_BRAKE,0.96);
    codeuseGauche = Codeuse(PIN_CODEUSE_GAUCHE_A,PIN_CODEUSE_GAUCHE_B);
    codeuseDroite = Codeuse(PIN_CODEUSE_DROITE_A,PIN_CODEUSE_DROITE_B);
    VectorE initVect = init_vectorE(x0,y0,theta0);
    ghost = *(new Ghost(initVect));
    posE.vec.x = x0;
    posE.vec.y=y0;
    posE.theta=theta0;
    vF = newFiltre(0.0,60.0,2);
    wF=newFiltre(0.0,60.0,2);
    ordresFifo.add(STBY(DYDM,Tirette,50,nullptr,simpleTimeout,1));
    master=new Cerveau(&ordresFifo);
    pid = PID(this);
    comm=*(new Comm());
}
