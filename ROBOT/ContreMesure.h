#ifndef CONTREMESURE_INCLUDED
#define CONTREMESURE_INCLUDED
#include "Robot.h"

bool simpleTimeout(Robot * ptrRobot,ErreurE erreur);  //Print toutes les erreurs dans le cas STATE. Passe a l'order suivante en cas de Timeout
bool normalTimeout(Robot * ptrRobot,ErreurE erreur);  //Print toutes les erreurs sauf les timeout dans le cas STATE. Passe a l'order suivante en cas de Timeout
bool resetGoto(Robot * ptrRobot,ErreurE erreur);      //Recalcul le Goto avec sanction. Passe sur un timeout
bool wiggle(Robot * ptrRobot,ErreurE erreur);         //Petit dandinage
bool jmeTire(Robot * ptrRobot,ErreurE erreur);          //Marche arriere qui remplace l'ordre actuel (un stby)
#endif


//ptrRobot->ordresFifo.add(SPIN(STD,PI,50,ptrPere,simpleTimeout,1));
//        ptrRobot->ordresFifo.add(GO_UNTIL(true,RECALLE,0.4,MessageE::Calle,100,ptrPere,simpleTimeout,1);
//        ptrRobot->ordresFifo.add(STBY(DYDM,Impossible,3,ptrPere,normalTimeout,1));
//        ptrRobot->ordresFifo.add(SETX(10,ptrPere,simpleTimeout,1));
//        ptrRobot->ordresFifo.add(GOTO(STD,0.1,0.1,0.0,0.0,true,50,ptrPere,simpleTimeout,1,false,true));
