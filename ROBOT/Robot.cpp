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
#include "Contacteur.h"

void Robot::actuate(float dt)
{
    codeuseGauche.actuate(dt);
    codeuseDroite.actuate(dt);
    actuateODO(dt);
    ghost.actuate(dt);
    pid.actuate(dt,posE,vF.out(),wF.out());
    comm.actuate(posE,vF.out());
    master->actuate();
    contacteurDroite.actuate();
    contacteurGauche.actuate();
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
    moteurGauche  = init_motor(PIN_MOTEUR_GAUCHE_PWR,PIN_MOTEUR_GAUCHE_SENS,PIN_MOTEUR_GAUCHE_BRAKE,1.0);
    codeuseGauche = Codeuse(PIN_CODEUSE_GAUCHE_A,PIN_CODEUSE_GAUCHE_B);
    codeuseDroite = Codeuse(PIN_CODEUSE_DROITE_A,PIN_CODEUSE_DROITE_B);
    contacteurDroite = Contacteur(PIN_CONTACTEUR_DROITE);
    contacteurGauche = Contacteur(PIN_CONTACTEUR_GAUCHE);
    VectorE initVect = init_vectorE(x0,y0,theta0);
    ghost = *(new Ghost(initVect));
    posE.vec.x = x0;
    posE.vec.y=y0;
    posE.theta=theta0;
    vF = newFiltre(0.0,100.0,2);
    wF=newFiltre(0.0,100.0,2);
    //ordresFifo.add(STBY(DYDM,Tirette,65000,nullptr,simpleTimeout,1));
    ordresFifo.add(STBY(DYDM,Tirette,60000,nullptr,simpleTimeout,1));
    ordresFifo.add(GOTO(RUSH,0.1,3.0,1,0,true,30,nullptr,simpleTimeout,1,false));
    ordresFifo.add(SPIN(RUSH,PI,50,nullptr,simpleTimeout,1));
    ordresFifo.add(GOTO(RUSH,0.3,1.6,1,PI,true,100,nullptr,simpleTimeout,1,false));
    ordresFifo.add(SPIN(RUSH,PI,50,nullptr,simpleTimeout,1));
    ordresFifo.add(STBY(DYDM,Tirette,65000,nullptr,simpleTimeout,1));
    master=new Cerveau(this);
    pid = PID(this);
    comm=*(new Comm());
}
