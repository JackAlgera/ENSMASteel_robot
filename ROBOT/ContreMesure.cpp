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
