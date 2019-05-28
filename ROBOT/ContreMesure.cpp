#include "Robot.h"
#include "ContreMesure.h"

bool simpleTimeout(Robot * ptrRobot,ErreurE erreur)
{
  #ifdef STATE
  Serial.print("Erreur : ");Serial.println(erreur);
  #endif
  if(erreur==ErreurE::TIMEOUT)
  {
    ptrRobot->pid.loadNext();
  }
  return false; //Pas de sanction
}

bool jmeTire(Robot * ptrRobot,ErreurE erreur)
{
    Vector devant=directeur(ptrRobot->posE.theta);
    Vector aim=add(ptrRobot->posE.vec,mult(-0.2,devant));
    ptrRobot->ordresFifo.replaceHead(GOTO(RUSH,0.1,aim.x,aim.y,ptrRobot->posE.theta,true,20,nullptr,simpleTimeout,1,AVOIDANCE));
    ptrRobot->pid.reload();
    return true;
}

bool normalTimeout(Robot * ptrRobot,ErreurE erreur)
{
  if(erreur==ErreurE::TIMEOUT)
  {
    ptrRobot->pid.loadNext();
  }
  else
  {
    #ifdef STATE
    Serial.print("Erreur : ");Serial.println(erreur);
    #endif
  }
  return false; //Pas de sanction
}

bool resetGoto(Robot * ptrRobot,ErreurE erreur)
{
  #ifdef STATE
  Serial.print("Erreur : ");Serial.println(erreur);
  #endif
  if(erreur==ErreurE::TIMEOUT)
  {
    ptrRobot->pid.loadNext();
    return false; //Pas de sanction
  }
  if(erreur==ErreurE::PID_FAILURE)
  {
    #ifdef STATE
    Serial.println("RESET GOTO ");
    #endif
    ptrRobot->pid.reload();
    return true;  //On sanctionne
  }
  return false;
}

bool wiggle(Robot * ptrRobot,ErreurE erreur)
{
  #ifdef STATE
  Serial.print("Erreur : ");Serial.println(erreur);
  #endif
  if(erreur==ErreurE::TIMEOUT)
  {
    ptrRobot->pid.loadNext();
    return false; //Pas de sanction
  }
  if(erreur==ErreurE::PID_FAILURE)
  {
    #ifdef STATE
    Serial.println("WIGGLE");
    #endif
    ptrRobot->ordresFifo.addHead(SPIN(STD,ptrRobot->posE.theta,10,nullptr,simpleTimeout,1));
    ptrRobot->ordresFifo.addHead(SPIN(STD,ptrRobot->posE.theta+0.1,10,nullptr,simpleTimeout,1));
    ptrRobot->pid.reload();
    return true;  //On sanctionne
  }
  return false;
}

bool recalleX(Robot * ptrRobot,ErreurE erreur)
{
    if(ptrRobot->posE.vec.x<1.5)
    {
        ptrRobot->ordresFifo.add(SPIN(STD,0.0,50,ptrPere,simpleTimeout,1));
        ptrRobot->ordresFifo.add(GO_UNTIL(true,RECALLE,0.4,MessageE::Calle,100,ptrPere,simpleTimeout,1);
        ptrRobot->ordresFifo.add(STBY(DYDM,Impossible,3,ptrPere,normalTimeout,1));
        ptrRobot->ordresFifo.add(SETX(10,ptrPere,simpleTimeout,1));
        ptrRobot->ordresFifo.add(GOTO(STD,0.1,0.1,0.0,0.0,true,50,ptrPere,simpleTimeout,1,false,true));
    }
    else
    {
        ptrRobot->ordresFifo.add(SPIN(STD,PI,50,ptrPere,simpleTimeout,1));
        ptrRobot->ordresFifo.add(GO_UNTIL(true,RECALLE,0.4,MessageE::Calle,100,ptrPere,simpleTimeout,1);
        ptrRobot->ordresFifo.add(STBY(DYDM,Impossible,3,ptrPere,normalTimeout,1));
        ptrRobot->ordresFifo.add(SETX(10,ptrPere,simpleTimeout,1));
        ptrRobot->ordresFifo.add(GOTO(STD,0.1,0.1,0.0,0.0,true,50,ptrPere,simpleTimeout,1,false,true));
    }
}
