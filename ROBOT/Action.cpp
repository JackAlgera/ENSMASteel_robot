#include "Action.h"


Action::Action(ActionE type, int nbrPalentsMax)
{
    this->type = type;
    this->nbrOrders = 0;
    this->currentOrderIndex = 0;
    this->actionCompleted = false;
  	this->nbrPalets = 0;
  	this->nbrPalentsMax = nbrPalentsMax;
}

Action::Action()
{
}

void Action::addGOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail, bool avoidance)
{
    ordersList[nbrOrders] = GOTO(nerv, fleche, xAim, yAim, thetaAim, arret, timeoutDs,this, contreMesure, nbMaxFail,avoidance);
    nbrOrders++;
}

void Action::addSPIN(uint8_t nerv, float thetaAim, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    ordersList[nbrOrders] = SPIN(nerv, thetaAim, timeoutDs,this, contreMesure, nbMaxFail);
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

void Action::addFWD(float acc, float v, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    ordersList[nbrOrders] = FWD(acc, v, timeoutDs,this, contreMesure, nbMaxFail);
    nbrOrders++;
}

void Action::addBWD(float acc, float v, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    ordersList[nbrOrders] = BWD(acc, v, timeoutDs,this, contreMesure, nbMaxFail);
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

bool Action::gotAllPalets()
{
	return nbrPalets == nbrPalentsMax;
}

void Action::addPalet()
{
  	if (nbrPalets < nbrPalentsMax)
  	{
  		  nbrPalets++;
  	}
}

void Action::addOrdersToBuffer(Fifo * ordresFifo)
{
    for(int i=0;i<nbrOrders;i++)					// On ajoute l'ensemble des ordres au buffer
    {
        ordresFifo->add(ordersList[i]);
    }
}
