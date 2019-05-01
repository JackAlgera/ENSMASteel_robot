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

void Action::set(ActionE type, int nbrOrders)
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
		Serial.print("Dans la fonction GOTO \n");
		Serial.print("Adresse action chaos : "); Serial.println((long unsigned int)this);
		ordersList[currentOrderAdd] = GOTO(nerv, fleche, xAim, yAim, thetaAim, arret, timeoutDs,this);
		Serial.print("Adr act depuis ordre : "); Serial.println((long unsigned int)ordersList[currentOrderAdd].ptrActionPere);
		currentOrderAdd++;
	}
}

void Action::addSPIN(uint8_t nerv, float thetaAim, uint8_t timeoutDs)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = SPIN(nerv, thetaAim, timeoutDs,this);
		currentOrderAdd++;
	}
}

void Action::addSPINGOTO(uint8_t nerv,float xAim, float yAim,uint8_t timeoutDs)
{
  if (currentOrderAdd < nbrOrders)
  {
    ordersList[currentOrderAdd] = SPINGOTO(nerv,xAim,yAim,timeoutDs,this);
    currentOrderAdd++;
  }
}

void Action::addFWD(float acc, float v, uint8_t timeoutDs)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = FWD(acc, v, timeoutDs,this);
		currentOrderAdd++;
	}
}

void Action::addBWD(float acc, float v, uint8_t timeoutDs)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = BWD(acc, v, timeoutDs,this);
		currentOrderAdd++;
	}
}

void Action::addSTBY(uint8_t nerv, const char unlockMessage[], uint8_t timeout)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = STBY(nerv, unlockMessage, timeout,this);
		currentOrderAdd++;
	}
}

void Action::addSEND(const char message[], uint8_t timeoutDs)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = SEND(message, timeoutDs,this);
		currentOrderAdd++;
	}
}

void Action::addEMSTOP(uint8_t timeoutDs)
{
	if (currentOrderAdd < nbrOrders)
	{
		ordersList[currentOrderAdd] = EMSTOP(timeoutDs,this);
		currentOrderAdd++;
	}
}

Order * Action::getCurrentOrder()
{
	return &ordersList[currentOrder];
}

void Action::nextStep()
{
	if (!actionCompleted)
	{
		Serial.print("l'adresse de l'action dont j'ai ++ le currentOrder: ");Serial.println((int)this);
    currentOrder++;
		if (currentOrder == nbrOrders)
		{
			actionCompleted = true;
		}
	}
}

void Action::addOrdersToBuffer(Fifo * ordresFifo, bool reAdd) // Si on souhaite re-ajouter la liste d'odres d'une action au buffer en partant du dernier ordre complete
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
