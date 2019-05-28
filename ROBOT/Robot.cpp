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
    if(contacteurDroite.isPressed() && contacteurGauche.isPressed())
        comm.lastMessage=MessageE::Calle;
    codeuseGauche.actuate(dt);
    codeuseDroite.actuate(dt);
    actuateODO(dt);
    ghost.actuate(dt);
    pid.actuate(dt,posE,vF.out(),wF.out());
    if (ordresFifo.ptrFst()->type==GOTO_E && ordresFifo.ptrFst()->goTo.avoidance)
    {
        comm.actuate(posE,(pid.jeVeuxAvancer)?(AnticolE::Front):(AnticolE::Back)  );
    }
    else if(evitting!=AnticolE::No)
    {
        comm.actuate(posE,evitting);
    }
    else
    {
        comm.actuate(posE,AnticolE::No);
    }
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
    evitting=AnticolE::No;
    moteurDroite  = init_motor(PIN_MOTEUR_DROITE_PWR,PIN_MOTEUR_DROITE_SENS,PIN_MOTEUR_DROITE_BRAKE,1.0);
    moteurGauche  = init_motor(PIN_MOTEUR_GAUCHE_PWR,PIN_MOTEUR_GAUCHE_SENS,PIN_MOTEUR_GAUCHE_BRAKE,1.0);
    codeuseGauche = Codeuse(PIN_CODEUSE_GAUCHE_A,PIN_CODEUSE_GAUCHE_B,DIAMETRE_ROUE_CODEUSE_GAUCHE);
    codeuseDroite = Codeuse(PIN_CODEUSE_DROITE_A,PIN_CODEUSE_DROITE_B,DIAMETRE_ROUE_CODEUSE_DROITE);
    contacteurDroite = Contacteur(PIN_CONTACTEUR_DROITE);
    contacteurGauche = Contacteur(PIN_CONTACTEUR_GAUCHE);
    VectorE initVect = init_vectorE(x0,y0,theta0);
    ghost = *(new Ghost(initVect));
    posE.vec.x = x0;
    posE.vec.y=y0;
    posE.theta=theta0;
    vF = newFiltre(0.0,100.0,2);
    wF=newFiltre(0.0,100.0,2);


    ordresFifo.add(STBY(DYDM,MessageE::Tirette,60000,nullptr,simpleTimeout,1));
    ordresFifo.add(GO_UNTIL(true,RECALLE,0.2,MessageE::Calle,200,nullptr,simpleTimeout,1));
    ordresFifo.add(SETX(10,nullptr,simpleTimeout,1));
    ordresFifo.add(GOTO(RUSH,0.1,0.1,0.0,0.0,true,50,nullptr,simpleTimeout,1,false,true));
    ordresFifo.add(STBY(DYDM,Impossible,65000,nullptr,simpleTimeout,1));

//    ordresFifo.add(SEND(MessageE::Pince_Half_Extended,1,nullptr,normalTimeout,1));
//    ordresFifo.add(STBY(RUSH,Impossible,2,nullptr,normalTimeout,1));
//
//    //ordresFifo.add(GOTO(STD,0.48,1.0671936758893281,0.97,-0.7621465405869852,true,100,nullptr,simpleTimeout,1,true));
//    ordresFifo.add(GOTO(RUSH,0.48,1.2,1.4,0,true,100,nullptr,simpleTimeout,1,true));
//
//    ordresFifo.add(GOTO(STD,0.2,0.2,1.4,0.0,true,100,nullptr,simpleTimeout,1,true));
//
//    ordresFifo.add(STBY(DYDM,Impossible,65000,nullptr,simpleTimeout,1));



    master=new Cerveau(this);
    pid = PID(this);
    comm=*(new Comm());
}

void waitGoto(Robot* robot)
{
    float dtLoop;
    uint32_t m,microsStart;
    m=micros();
    microsStart=(uint32_t)((float)m-(1.0/FREQUENCY)*1000000.0);
    while (robot->ordresFifo.ptrFst()->type!=OrderE::GOTO_E)       //On l'execute
    {
        m=micros();
        dtLoop=(m-microsStart)/1000000.0;
        microsStart=m;
        robot->actuate(dtLoop);
        while((micros()-microsStart)/1000000.0<1.0/FREQUENCY) {;}
    }
}

void doGoto(Robot* robot)
{
    float dtLoop;
    uint32_t m,microsStart;
    m=micros();
    microsStart=(uint32_t)((float)m-(1.0/FREQUENCY)*1000000.0);
    robot->ghost.locked=true;
    while (robot->ordresFifo.ptrFst()->type==OrderE::GOTO_E)       //On l'execute
    {
        m=micros();
        dtLoop=(m-microsStart)/1000000.0;
        microsStart=m;
        robot->actuate(dtLoop);
        while((micros()-microsStart)/1000000.0<1.0/FREQUENCY) {;}
    }
}

void waitSpin(Robot* robot)
{
    float dtLoop;
    uint32_t m,microsStart;
    m=micros();
    microsStart=(uint32_t)((float)m-(1.0/FREQUENCY)*1000000.0);
    while (robot->ordresFifo.ptrFst()->type!=OrderE::SPIN_E)       //On l'execute
    {
        m=micros();
        dtLoop=(m-microsStart)/1000000.0;
        microsStart=m;
        robot->actuate(dtLoop);
        while((micros()-microsStart)/1000000.0<1.0/FREQUENCY) {;}
    }
}

void doSpin(Robot* robot)
{
    float dtLoop;
    uint32_t m,microsStart;
    m=micros();
    microsStart=(uint32_t)((float)m-(1.0/FREQUENCY)*1000000.0);
    robot->ghost.locked=true;
    while (robot->ordresFifo.ptrFst()->type==OrderE::SPIN_E)       //On l'execute
    {
        m=micros();
        dtLoop=(m-microsStart)/1000000.0;
        microsStart=m;
        robot->actuate(dtLoop);
        while((micros()-microsStart)/1000000.0<1.0/FREQUENCY) {;}
    }
}



void Robot::autoTune(uint8_t nerv,uint8_t coeff,float delta,uint8_t type)
{
    delay(2000);
    bool Continue=true;
    uint32_t m;
    m=micros();
    float lastLoss,newLoss;
    ordresFifo.clean();
    ordresFifo.add(SEND(MessageE::Pince_Retracted,20,nullptr,simpleTimeout,1));
    delay(1000);
    if (type==LIN)
    {
        if(nerv!=DYDM)
        {
            ordresFifo.add(STBY(OFF,Impossible,5,nullptr,normalTimeout,1));
            ordresFifo.add(GOTO(nerv,0.3,2.2,1.4,0.0,true,80,nullptr,simpleTimeout,1,false));
            waitGoto(this);
            doGoto(this);
            lastLoss=pid.lastLossLin/2.0;
            ordresFifo.add(STBY(OFF,Impossible,5,nullptr,normalTimeout,1));
            ordresFifo.add(GOTO(nerv,0.3,0.2,1.4,0.0,true,80,nullptr,simpleTimeout,1,false));
            waitGoto(this);
            doGoto(this);
            lastLoss+=pid.lastLossLin/2.0;
            while (Continue)
            {
                pid.increment(nerv,coeff,type,delta);   //On change les coeff

                //Aller
                ordresFifo.add(STBY(OFF,Impossible,5,nullptr,normalTimeout,1));
                ordresFifo.add(GOTO(nerv,0.3,2.2,1.4,0.0,true,80,nullptr,simpleTimeout,1,false));
                waitGoto(this);
                doGoto(this);
                newLoss=pid.lastLossLin/2.0;

                //Retour
                ordresFifo.add(STBY(OFF,Impossible,5,nullptr,normalTimeout,1));
                ordresFifo.add(GOTO(nerv,0.3,0.2,1.4,0.0,true,80,nullptr,simpleTimeout,1,false));
                waitGoto(this);
                doGoto(this);
                newLoss+=pid.lastLossLin/2.0;
                if (newLoss>lastLoss)
                {
                    pid.decrement(nerv,coeff,type,delta);
                    Continue=false;
                }
                else
                    lastLoss=newLoss;
            }
        }
        else
        {}

    }
    else    //ANG
    {
        if (nerv!=DYDM)
        {
            ordresFifo.add(STBY(OFF,Impossible,5,nullptr,normalTimeout,1));
            ordresFifo.add(SPIN(nerv,PI*99.0/100.0,50,nullptr,simpleTimeout,1,true));
            waitSpin(this);
            doSpin(this);
            lastLoss=pid.lastLossAng/2.0;
            ordresFifo.add(STBY(OFF,Impossible,5,nullptr,normalTimeout,1));
            ordresFifo.add(SPIN(nerv,-PI*99.0/100.0,50,nullptr,simpleTimeout,1,true));
            waitSpin(this);
            doSpin(this);
            lastLoss+=pid.lastLossAng/2.0;
            while (Continue)
            {
                pid.increment(nerv,coeff,type,delta);   //On change les coeff

                //Aller
                ordresFifo.add(STBY(OFF,Impossible,5,nullptr,normalTimeout,1));
                ordresFifo.add(SPIN(nerv,PI*99.0/100.0,50,nullptr,simpleTimeout,1,true));  //On refait le goto
                waitSpin(this);
                doSpin(this);
                newLoss=pid.lastLossAng/2.0;

                //Retour
                ordresFifo.add(STBY(OFF,Impossible,5,nullptr,normalTimeout,1));
                ordresFifo.add(SPIN(nerv,-PI*99.0/100.0,50,nullptr,simpleTimeout,1,true));  //On refait le goto
                waitSpin(this);
                doSpin(this);
                newLoss+=pid.lastLossAng/2.0;
                if (newLoss>lastLoss)
                {
                    pid.decrement(nerv,coeff,type,delta);
                    Continue=false;
                }
                else
                    lastLoss=newLoss;
            }
        }
        else
        {
            ordresFifo.add(STBY(OFF,Impossible,5,nullptr,normalTimeout,1));
            ordresFifo.add(GOTO(RUSH,0.3,2.2,1.4,0.0,true,80,nullptr,simpleTimeout,1,false));
            waitGoto(this);
            doGoto(this);
            lastLoss=pid.lastLossAng/2.0;
            ordresFifo.add(STBY(OFF,Impossible,5,nullptr,normalTimeout,1));
            ordresFifo.add(GOTO(RUSH,0.3,0.2,1.4,0.0,true,80,nullptr,simpleTimeout,1,false));
            waitGoto(this);
            doGoto(this);
            lastLoss+=pid.lastLossAng/2.0;
            while (Continue)
            {
                pid.increment(nerv,coeff,type,delta);   //On change les coeff

                //Aller
                ordresFifo.add(STBY(OFF,Impossible,5,nullptr,normalTimeout,1));
                ordresFifo.add(GOTO(RUSH,0.3,2.2,1.4,0.0,true,80,nullptr,simpleTimeout,1,false));
                waitGoto(this);
                doGoto(this);
                newLoss=pid.lastLossAng/2.0;

                //Retour
                ordresFifo.add(STBY(OFF,Impossible,5,nullptr,normalTimeout,1));
                ordresFifo.add(GOTO(RUSH,0.3,0.2,1.4,0.0,true,80,nullptr,simpleTimeout,1,false));
                waitGoto(this);
                doGoto(this);
                newLoss+=pid.lastLossAng/2.0;
                if (newLoss>lastLoss)
                {
                    pid.decrement(nerv,coeff,type,delta);
                    Continue=false;
                }
                else
                    lastLoss=newLoss;
            }
        }
    }
}
