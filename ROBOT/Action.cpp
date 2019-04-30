#include "Action.h"

Action::Action(ActionE type, int nbrOrders)
{
	this->type = type;
	this->nbrOrders = nbrOrders;
	this->currentOrder = 0;
	this->actionCompleted = false;
	this->currentOrderAdd = 0;
	this->currentBufferOrder = 0;
	ordersList = new Order[nbrOrders];
}

Action::Action()
{
}

void Action::addGOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint8_t timeoutDs)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = GOTO(nerv, fleche, xAim, yAim, thetaAim, arret, timeoutDs);
		currentOrderAdd++;
	}
}

void Action::addSPIN(uint8_t nerv, float thetaAim, uint8_t timeoutDs)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = SPIN(nerv, thetaAim, timeoutDs);
		currentOrderAdd++;
	}
}

void Action::addSPINGOTO(uint8_t nerv,float xAim, float yAim,uint8_t timeoutDs)
{
  if (currentOrderAdd < nbrOrders)
  {
    ordersList[currentOrderAdd] = SPINGOTO(nerv,xAim,yAim,timeoutDs);
    currentOrderAdd++;
  }
}

void Action::addFWD(float acc, float v, uint8_t timeoutDs)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = FWD(acc, v, timeoutDs);
		currentOrderAdd++;
	}
}

void Action::addBWD(float acc, float v, uint8_t timeoutDs)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = BWD(acc, v, timeoutDs);
		currentOrderAdd++;
	}
}

void Action::addSTBY(uint8_t nerv, const char unlockMessage[], uint8_t timeout)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = STBY(nerv, unlockMessage, timeout);
		currentOrderAdd++;
	}
}

void Action::addPOST(const char message[], uint8_t timeoutDs)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = POST(message, timeoutDs);
		currentOrderAdd++;
	}
}

void Action::addEMSTOP(uint8_t timeoutDs)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = EMSTOP(timeoutDs);
		currentOrderAdd++;
	}
}

Order * Action::getCurrentOrder()
{
	return &ordersList[currentOrder];
}

bool Action::finirOrder()
{
	if (!actionCompleted)
	{
		currentOrder++;
		if (currentOrder == nbrOrders)
		{
			actionCompleted = true;
			return actionCompleted;
		}
	}
	return false;
}

void Action::addOrdersToBuffer(Fifo * ordresFifo, bool reAdd = false) // Si on souhaite re-ajouter la liste d'odres d'une action au buffer en partant du dernier ordre complete
{
	if (reAdd)
	{
		currentBufferOrder = currentOrder;
	}

	while (currentBufferOrder < nbrOrders)					// On ajoute l'ensemble des ordres au buffer
	{
		ordresFifo->add(ordersList[currentBufferOrder]);
		currentBufferOrder++;
	}
}
