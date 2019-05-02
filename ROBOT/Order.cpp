#include "Order.h"

Order::Order()
{
}

SPIN::SPIN(uint8_t nerv, float thetaAim, uint8_t timeoutDs, Action * ptrActionPere)
{
	{
		this->type = OrderE::SPIN_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;

		this->spin = { nerv, thetaAim };
	}
}

GOTO::GOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint8_t timeoutDs, Action * ptrActionPere)
{ //abs(thetaAim-thetaIni)<=PI
	{
		this->type = OrderE::GOTO_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;

		this->goTo = { nerv, fleche, xAim, yAim, thetaAim, arret };
	}
}

SPINGOTO::SPINGOTO(uint8_t nerv, float xAim, float yAim, uint8_t timeoutDs, Action * ptrActionPere)
{
	{
		this->type = OrderE::SPINGOTO_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;

		this->spinGoTo.nerv = nerv;
		this->spinGoTo.posAim = init_vector(xAim, yAim);
	}
}

FWD::FWD(float acc, float v, uint8_t timeoutDs, Action * ptrActionPere)
{
	{
		this->type = OrderE::FWD_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;

		this->fwd = { acc, v };
	}
}

BWD::BWD(float acc, float v, uint8_t timeoutDs, Action * ptrActionPere)
{
	{
		this->type = OrderE::BWD_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;

		this->bwd = { acc, v };
	}
}

STBY::STBY(uint8_t nerv, const char unlockMessage[], uint8_t timeoutDs, Action * ptrActionPere)
{
	{
		this->type = OrderE::STBY_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;

		this->stby.nerv = nerv;
		this->stby.unlockMessage[0] = unlockMessage[0];
		this->stby.unlockMessage[1] = unlockMessage[1];
		this->stby.unlockMessage[2] = unlockMessage[2];
		this->stby.unlockMessage[3] = unlockMessage[3];
	}
}

SEND::SEND(const char message[], uint8_t timeoutDs, Action * ptrActionPere)
{
	{
		this->type = OrderE::SEND_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;

		this->send.message[0] = message[0];
		this->send.message[1] = message[1];
		this->send.message[2] = message[2];
		this->send.message[3] = message[3];
	}
}

EMSTOP::EMSTOP(uint8_t timeoutDs, Action * ptrActionPere)
{
	{
		this->type = OrderE::EMSTOP_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;
	}
}
