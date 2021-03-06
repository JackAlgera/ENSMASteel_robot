#include "Order.h"
#include "ContreMesure.h"

Order::Order()
{
}

SPIN::SPIN(uint8_t nerv, float thetaAim, uint8_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
	{
		this->type = OrderE::SPIN_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;
    this->contreMesure=contreMesure;
    this->nbMaxFail=nbMaxFail;
    
    this->nbFail=0;

		this->spin = { nerv, thetaAim };
	}
}

GOTO::GOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint8_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{ //abs(thetaAim-thetaIni)<=PI
	{
		this->type = OrderE::GOTO_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;
    this->contreMesure=contreMesure;
    this->nbMaxFail=nbMaxFail;
    
    this->nbFail=0;

		this->goTo = { nerv, fleche, xAim, yAim, thetaAim, arret };
	}
}

SPINGOTO::SPINGOTO(uint8_t nerv, float xAim, float yAim, uint8_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
	{
		this->type = OrderE::SPINGOTO_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;
    this->contreMesure=contreMesure;
    this->nbMaxFail=nbMaxFail;
    
    this->nbFail=0;

		this->spinGoTo.nerv = nerv;
		this->spinGoTo.posAim = init_vector(xAim, yAim);
	}
}

FWD::FWD(float acc, float v, uint8_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
	{
		this->type = OrderE::FWD_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;
    this->contreMesure=contreMesure;
    this->nbMaxFail=nbMaxFail;
    
    this->nbFail=0;

		this->fwd = { acc, v };
	}
}

BWD::BWD(float acc, float v, uint8_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
	{
		this->type = OrderE::BWD_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;
    this->contreMesure=contreMesure;
    this->nbMaxFail=nbMaxFail;
    
    this->nbFail=0;

		this->bwd = { acc, v };
	}
}

STBY::STBY(uint8_t nerv, MessageE unlockMessage, uint8_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
	{
		this->type = OrderE::STBY_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;
    this->contreMesure=contreMesure;
    this->nbMaxFail=nbMaxFail;
    
    this->nbFail=0;

		this->stby.nerv = nerv;
		this->stby.unlockMessage = unlockMessage;
	}
}

SEND::SEND(MessageE message, uint8_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
	{
		this->type = OrderE::SEND_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;
    this->contreMesure=contreMesure;
    this->nbMaxFail=nbMaxFail;
    
    this->nbFail=0;

		this->send.message = message;
	}
}

EMSTOP::EMSTOP(uint8_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
	{
		this->type = OrderE::EMSTOP_E;
		this->timeoutDs = timeoutDs;
		this->ptrActionPere = ptrActionPere;
    this->contreMesure=contreMesure;
    this->nbMaxFail=nbMaxFail;
    
    this->nbFail=0;
	}
}
