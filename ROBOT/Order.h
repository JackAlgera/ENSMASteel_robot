#ifndef ORDER_INCLUDED
#define ORDER_INCLUDED

#include "1_CONSTANT.h"
#include "Vector.h"
#include "Arduino.h"
class Action;
class Robot;

typedef void (*ptrFonction)(Robot * ptrRobot);


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
    ptrFonction contreMesure;
    uint8_t nbFail;
    uint8_t nbMaxFail;
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
	GOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint8_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail);
};

class SPIN : public Order
{
public:
	SPIN(uint8_t nerv, float thetaAim, uint8_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class SPINGOTO : public Order
{
public:
	SPINGOTO(uint8_t nerv, float xAim, float yAim, uint8_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class FWD : public Order
{
public:
	FWD(float acc, float v, uint8_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class BWD : public Order
{
public:
	BWD(float acc, float v, uint8_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class STBY : public Order
{
public:
    uint8_t nerv;
    char unlockMessage[4];

	STBY(uint8_t nerv, const char unlockMessage[], uint8_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class SEND : public Order
{
public:
	SEND(const char message[], uint8_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class EMSTOP : public Order
{
public:
	EMSTOP(uint8_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

#endif
