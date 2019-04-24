#include "Action.h"

Action::Action(ActionE type, int nbrOrders)
{
	this->type = type;
	this->nbrOrders = nbrOrders;
	ordersList = new Order_new[nbrOrders];
}

inline void Action::addGoto(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint8_t timeoutDs)
{
	if (currentOrder < nbrOrders)
	{
		ordersList[currentOrderAdd] = GoTo(nerv, fleche, xAim, yAim, thetaAim, arret, timeoutDs);
		currentOrderAdd++;
	}
}

inline void Action::addSpin(uint8_t nerv, float thetaAim, uint8_t timeoutDs)
{
	if (currentOrder < nbrOrders)
	{
		ordersList[currentOrderAdd] = Spin(nerv, thetaAim, timeoutDs);
		currentOrderAdd++;
	}
}

inline void Action::addFWD(float acc, float v, uint8_t timeoutDs)
{
	if (currentOrder < nbrOrders)
	{
		ordersList[currentOrderAdd] = FWD(acc, v, timeoutDs);
		currentOrderAdd++;
	}
}

inline void Action::addBWD(float acc, float v, uint8_t timeoutDs)
{
	if (currentOrder < nbrOrders)
	{
		ordersList[currentOrderAdd] = BWD(acc, v, timeoutDs);
		currentOrderAdd++;
	}
}

void Action::addSTBY(uint8_t nerv, const char unlockMessage[], uint8_t timeout)
{
	if (currentOrder < nbrOrders)
	{
		ordersList[currentOrderAdd] = STBY_new(nerv, unlockMessage, timeout);
		currentOrderAdd++;
	}
}

Order_new * Action::getCurrentOrder()
{
	return &ordersList[currentOrder];
}
