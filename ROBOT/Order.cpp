#include "Order.h"
#include "ContreMesure.h"

Order::Order()
{
}

SPIN::SPIN(uint8_t nerv, float thetaAim, uint16_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail,bool relativ)
{
    {
        this->type = OrderE::SPIN_E;
        this->errorIntegralLin=0;
        this->errorIntegralAng=0;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere = ptrActionPere;
        this->contreMesure=contreMesure;
        this->nbMaxFail=nbMaxFail;

        this->nbFail=0;

        this->spin = { nerv, thetaAim,relativ };
    }
}

GOTO::GOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint16_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail,bool avoidance,bool relativ)
{
    //abs(thetaAim-thetaIni)<=PI
    {
        this->type = OrderE::GOTO_E;
        this->errorIntegralLin=0;
        this->errorIntegralAng=0;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere = ptrActionPere;
        this->contreMesure=contreMesure;
        this->nbMaxFail=nbMaxFail;

        this->nbFail=0;

        this->goTo = { nerv, fleche, xAim, yAim, thetaAim, arret,avoidance,relativ };
    }
}

SPINTO::SPINTO(uint8_t nerv, float xAim, float yAim, uint16_t timeoutDs, Action* ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    this->type = OrderE::SPINTO_E;
    this->errorIntegralLin=0;
    this->errorIntegralAng=0;
    this->timeoutDs = timeoutDs;
    this->ptrActionPere = ptrActionPere;
    this->contreMesure=contreMesure;
    this->nbMaxFail=nbMaxFail;
    this->nbFail=0;
    this->spinTo = { nerv, xAim, yAim};
}


SPINGOTO::SPINGOTO(uint8_t nerv, float xAim, float yAim, uint16_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail,bool avoidance)
{
    {
        this->type = OrderE::SPINGOTO_E;
        this->errorIntegralLin=0;
        this->errorIntegralAng=0;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere = ptrActionPere;
        this->contreMesure=contreMesure;
        this->nbMaxFail=nbMaxFail;

        this->nbFail=0;

        this->spinGoTo.nerv = nerv;
        this->spinGoTo.posAim = init_vector(xAim, yAim);
        this->spinGoTo.avoidance=avoidance;
    }

}

GO_UNTIL::GO_UNTIL(bool arriere,uint8_t nerv,float distanceMax,MessageE unlockMessage, uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail)
{
    {
        this->type = OrderE::GO_UNTIL_E;
        this->errorIntegralLin=0;
        this->errorIntegralAng=0;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere = ptrActionPere;
        this->contreMesure=contreMesure;
        this->nbMaxFail=nbMaxFail;

        this->nbFail=0;

        this->go_until = { arriere,nerv,distanceMax,unlockMessage };
    }
}


STBY::STBY(uint8_t nerv, MessageE unlockMessage, uint16_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    {
        this->type = OrderE::STBY_E;
        this->errorIntegralLin=0;
        this->errorIntegralAng=0;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere = ptrActionPere;
        this->contreMesure=contreMesure;
        this->nbMaxFail=nbMaxFail;

        this->nbFail=0;

        this->stby.nerv = nerv;
        this->stby.unlockMessage = unlockMessage;
    }
}

SEND::SEND(MessageE message, uint16_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    {
        this->type = OrderE::SEND_E;
        this->errorIntegralLin=0;
        this->errorIntegralAng=0;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere = ptrActionPere;
        this->contreMesure=contreMesure;
        this->nbMaxFail=nbMaxFail;

        this->nbFail=0;

        this->send.message = message;
    }
}

EMSTOP::EMSTOP(uint16_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    {
        this->type = OrderE::EMSTOP_E;
        this->errorIntegralLin=0;
        this->errorIntegralAng=0;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere = ptrActionPere;
        this->contreMesure=contreMesure;
        this->nbMaxFail=nbMaxFail;

        this->nbFail=0;
    }
}

SETX::SETX(float xValue,float theta,uint16_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    {
        this->type = OrderE::SETX_E;
        this->errorIntegralLin=0;
        this->errorIntegralAng=0;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere = ptrActionPere;
        this->contreMesure=contreMesure;
        this->nbMaxFail=nbMaxFail;

        this->nbFail=0;
        setx.xValue=xValue;
        setx.theta=theta;
    }
}

SETY::SETY(float yValue,float theta,uint16_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail)
{
    {
        this->type = OrderE::SETY_E;
        this->errorIntegralLin=0;
        this->errorIntegralAng=0;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere = ptrActionPere;
        this->contreMesure=contreMesure;
        this->nbMaxFail=nbMaxFail;

        this->nbFail=0;
        this->sety.yValue=yValue;
        this->sety.theta=theta;
    }
}
