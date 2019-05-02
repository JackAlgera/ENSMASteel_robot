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
