#include "Action.h"

Action::Action(ActionE type, int nbrOrders)
{
	this->type = type;
	this->nbrOrders = nbrOrders;
	ordersList = new Order*[nbrOrders];
}

Action::Action()
{
}

void Action::addGoTo(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint8_t timeoutDs)
{
	if (currentOrder < nbrOrders)
	{
		ordersList[currentOrderAdd] = new GoTo(nerv, fleche, xAim, yAim, thetaAim, arret, timeoutDs);
		currentOrderAdd++;
	}
}

void Action::addSpin(uint8_t nerv, float thetaAim, uint8_t timeoutDs)
{
	if (currentOrder < nbrOrders)
	{
		ordersList[currentOrderAdd] = new Spin(nerv, thetaAim, timeoutDs);
		currentOrderAdd++;
	}
}

void Action::addFWD(float acc, float v, uint8_t timeoutDs)
{
	if (currentOrder < nbrOrders)
	{
		ordersList[currentOrderAdd] = new FWD(acc, v, timeoutDs);
		currentOrderAdd++;
	}
}

void Action::addBWD(float acc, float v, uint8_t timeoutDs)
{
	if (currentOrder < nbrOrders)
	{
		ordersList[currentOrderAdd] = new BWD(acc, v, timeoutDs);
		currentOrderAdd++;
	}
}

void Action::addSTBY(uint8_t nerv, const char unlockMessage[], uint8_t timeout)
{
	if (currentOrder < nbrOrders)
	{
		ordersList[currentOrderAdd] = new STBY(nerv, unlockMessage, timeout);
		currentOrderAdd++;
	}
}

Order * Action::getCurrentOrder()
{
	return ordersList[currentOrder];
}
