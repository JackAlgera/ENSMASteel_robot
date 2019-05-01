#ifndef ORDER_INCLUDED
#define ORDER_INCLUDED

#include "1_CONSTANT.h"
#include "Vector.h"
#include "Arduino.h"
class Action;

struct GOTO_S
{
    uint8_t nerv;
    float fleche;         //La flèche est sans unité. Elle indique de combien le robot peut d'éloigner du trajet le plus court
    Vector posAim;        //Position cibe
    float thetaAim;       //Deceleration, angle cible, vitesse cible
    bool arret;
};

struct SPIN_S
{
    uint8_t nerv;
    float thetaAim;       //angle cible, acceleration et deceleration angulaire (theta point point)
};

struct SPINGOTO_S
{
    uint8_t nerv;
    Vector posAim;
};

struct FWD_S
{
    float acc, v;          //Acceleration, vitesse max, temps maximum passé sur cette instruction
};

struct BWD_S
{
    float acc, v;
};

struct STBY_S
{
    uint8_t nerv;
    char unlockMessage[4];
};

struct SEND_S
{
    char message[4];
};

struct EMSTOP_S
{
};

class Order
{
public:
    OrderE type;
    uint8_t timeoutDs;
    Action * ptrActionPere;
    union                         //Une union permet de dire que uniquement l'un de ces champs à un intéret
    {
        GOTO_S goTo;
        SPIN_S spin;
        SPINGOTO_S spinGoTo;
        FWD_S fwd;
        BWD_S bwd;
        STBY_S stby;
        SEND_S send;
        EMSTOP_S emStop;
    };

    Order();
};

class GOTO : public Order
{
public:
    GOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint8_t timeoutDs, Action * ptrActionPere) //abs(thetaAim-thetaIni)<=PI
    {
        this->type = OrderE::GOTO_E;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere=ptrActionPere;

        this->goTo = { nerv, fleche, xAim, yAim, thetaAim, arret };
    }
};

class SPIN : public Order
{
public:
    SPIN(uint8_t nerv, float thetaAim, uint8_t timeoutDs, Action * ptrActionPere)
    {
        this->type = OrderE::SPIN_E;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere=ptrActionPere;

        this->spin = { nerv, thetaAim };
    }
};

class SPINGOTO : public Order
{
public:
    SPINGOTO(uint8_t nerv, float xAim, float yAim, uint8_t timeoutDs, Action * ptrActionPere)
    {
        this->type = OrderE::SPINGOTO_E;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere=ptrActionPere;

        this->spinGoTo.nerv=nerv;
        this->spinGoTo.posAim=init_vector(xAim,yAim);
    }
};

class FWD : public Order
{
public:
    FWD(float acc, float v, uint8_t timeoutDs, Action * ptrActionPere)
    {
        this->type = OrderE::FWD_E;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere=ptrActionPere;

        this->fwd = { acc, v };
    }
};

class BWD : public Order
{
public:
    BWD(float acc, float v, uint8_t timeoutDs, Action * ptrActionPere)
    {
        this->type = OrderE::BWD_E;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere=ptrActionPere;

        this->bwd = { acc, v };
    }
};

class STBY : public Order
{
public:
    uint8_t nerv;
    char unlockMessage[4];

    STBY(uint8_t nerv, const char unlockMessage[], uint8_t timeoutDs, Action * ptrActionPere)
    {
        this->type = OrderE::STBY_E;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere=ptrActionPere;

        this->stby.nerv = nerv;
        this->stby.unlockMessage[0] = unlockMessage[0];
        this->stby.unlockMessage[1] = unlockMessage[1];
        this->stby.unlockMessage[2] = unlockMessage[2];
        this->stby.unlockMessage[3] = unlockMessage[3];
    }
};

class SEND : public Order
{
public:
    SEND(const char message[], uint8_t timeoutDs, Action * ptrActionPere)
    {
        this->type = OrderE::SEND_E;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere=ptrActionPere;

        this->send.message[0] = message[0];
        this->send.message[1] = message[1];
        this->send.message[2] = message[2];
        this->send.message[3] = message[3];
    }
};

class EMSTOP : public Order
{
public:
    EMSTOP(uint8_t timeoutDs,Action * ptrActionPere)
    {
        this->type = OrderE::EMSTOP_E;
        this->timeoutDs = timeoutDs;
        this->ptrActionPere=ptrActionPere;
    }
};

#endif
