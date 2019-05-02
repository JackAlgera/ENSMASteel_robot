#ifndef CONTREMESURE_INCLUDED
#define CONTREMESURE_INCLUDED
#include "Robot.h"


bool simpleTimeout(Robot * ptrRobot,ErreurE erreur);  //Print toutes les erreurs dans le cas STATE. Passe a l'order suivante en cas de Timeout
bool normalTimeout(Robot * ptrRobot,ErreurE erreur);  //Print toutes les erreurs sauf les timeout dans le cas STATE. Passe a l'order suivante en cas de Timeout

#endif
