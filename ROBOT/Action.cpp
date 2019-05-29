#include "Action.h"


Action::Action(ActionE type)
{
    this->type = type;
    this->nbrOrders = 0;
    this->currentOrderIndex = 0;
    this->actionCompleted = false;
}

Action::Action()
{
}

void Action::addGOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail, bool avoidance,bool relativ)
{
    ordersList[nbrOrders] = GOTO(nerv, fleche, xAim, yAim, thetaAim, arret, timeoutDs,this, contreMesure, nbMaxFail,avoidance,relativ);
    nbrOrders++;
}

void Action::addGOTO(uint8_t nerv, float fleche, VectorE aim, bool arret, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail, bool avoidance,bool relativ)
{
    addGOTO(nerv,fleche, aim.vec.x, aim.vec.y, aim.theta, arret, timeoutDs, contreMesure, nbMaxFail, avoidance,relativ);
}

void Action::addSPIN(uint8_t nerv, float thetaAim, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail,bool relativ)
{
    ordersList[nbrOrders] = SPIN(nerv, thetaAim, timeoutDs,this, contreMesure, nbMaxFail,relativ);
    nbrOrders++;
}

void Action::addSPINTO(uint8_t nerv, float xAim, float yAim, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    ordersList[nbrOrders] = SPINTO(nerv, xAim, yAim,timeoutDs, this, contreMesure, nbMaxFail);
    nbrOrders++;
}


void Action::addSPINGOTO(uint8_t nerv,float xAim, float yAim,uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail,bool avoidance)
{
    ordersList[nbrOrders] = SPINGOTO(nerv,xAim,yAim,timeoutDs,this, contreMesure, nbMaxFail,avoidance);
    nbrOrders++;
}

void Action::addGO_UNTIL(bool arriere,uint8_t nerv,float distanceMax,MessageE unlockMessage, uint16_t timeoutDs,ptrFonction contreMesure,uint8_t nbMaxFail)
{
    ordersList[nbrOrders] = GO_UNTIL(arriere,nerv,distanceMax,unlockMessage,timeoutDs,this,contreMesure,nbMaxFail);
    nbrOrders++;
}


void Action::addSTBY(uint8_t nerv, MessageE unlockMessage, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    ordersList[nbrOrders] = STBY(nerv, unlockMessage, timeoutDs,this, contreMesure, nbMaxFail);
    nbrOrders++;
}

void Action::addSEND(MessageE message, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    ordersList[nbrOrders] = SEND(message, timeoutDs,this, contreMesure, nbMaxFail);
    nbrOrders++;
}

void Action::addSETX(float xValue, float theta, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    ordersList[nbrOrders] = SETX(xValue,theta,timeoutDs,this,contreMesure,1);
    nbrOrders++;
}

void Action::addSETY(float yValue, float theta, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    ordersList[nbrOrders] = SETY(yValue,theta,timeoutDs,this,contreMesure,1);
    nbrOrders++;
}


void Action::addEMSTOP(uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    ordersList[nbrOrders] = EMSTOP(timeoutDs,this, contreMesure, nbMaxFail);
    nbrOrders++;
}

Order * Action::getCurrentOrder()
{
    return &ordersList[currentOrderIndex];
}

void Action::nextStep()
{
    if (!actionCompleted)
    {
        currentOrderIndex++;
        if (currentOrderIndex == nbrOrders)
        {
            actionCompleted = true;
        }
    }
}


void Action::addOrdersToBuffer(Fifo * ordresFifo)
{
    for(int i=0;i<nbrOrders;i++)					// On ajoute l'ensemble des ordres au buffer
    {
        ordresFifo->add(ordersList[i]);
    }
}
