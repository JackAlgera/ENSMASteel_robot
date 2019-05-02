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

void Action::addGOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint8_t timeoutDs)
{
    ordersList[nbrOrders] = GOTO(nerv, fleche, xAim, yAim, thetaAim, arret, timeoutDs,this);
    nbrOrders++;
}

void Action::addSPIN(uint8_t nerv, float thetaAim, uint8_t timeoutDs)
{
    ordersList[nbrOrders] = SPIN(nerv, thetaAim, timeoutDs,this);
    nbrOrders++;
}

void Action::addSPINGOTO(uint8_t nerv,float xAim, float yAim,uint8_t timeoutDs)
{
    ordersList[nbrOrders] = SPINGOTO(nerv,xAim,yAim,timeoutDs,this);
    nbrOrders++;
}

void Action::addFWD(float acc, float v, uint8_t timeoutDs)
{
    ordersList[nbrOrders] = FWD(acc, v, timeoutDs,this);
    nbrOrders++;
}

void Action::addBWD(float acc, float v, uint8_t timeoutDs)
{
    ordersList[nbrOrders] = BWD(acc, v, timeoutDs,this);
    nbrOrders++;
}

void Action::addSTBY(uint8_t nerv, const char unlockMessage[], uint8_t timeout)
{
    ordersList[nbrOrders] = STBY(nerv, unlockMessage, timeout,this);
    nbrOrders++;
}

void Action::addSEND(const char message[], uint8_t timeoutDs)
{
    ordersList[nbrOrders] = SEND(message, timeoutDs,this);
    nbrOrders++;
}

void Action::addEMSTOP(uint8_t timeoutDs)
{
    ordersList[nbrOrders] = EMSTOP(timeoutDs,this);
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
