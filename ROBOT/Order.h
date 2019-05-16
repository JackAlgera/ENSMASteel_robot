#ifndef ORDER_INCLUDED
#define ORDER_INCLUDED

#include "1_CONSTANT.h"
#include "Vector.h"
#include "Arduino.h"
class Action;
class Robot;

typedef bool (*ptrFonction)(Robot * ptrRobot,ErreurE erreur);   //Renvoie la necessite de sanctionner l'erreur ou non


struct GOTO_S
{
    uint8_t nerv;
    float fleche;         //La flèche est sans unité. Elle indique de combien le robot peut d'éloigner du trajet le plus court
    Vector posAim;        //Position cibe
    float thetaAim;       //Deceleration, angle cible, vitesse cible
    bool arret,avoidance;
};

struct SPIN_S
{
    uint8_t nerv;
    float thetaAim;       //angle cible, acceleration et deceleration angulaire (theta point point)
};

struct SPINTO_S
{
    uint8_t nerv;
    float xAim,yAim;
};

struct SPINGOTO_S
{
    uint8_t nerv;
    Vector posAim;
    bool avoidance;
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
    MessageE unlockMessage;
};

struct SEND_S
{
    MessageE message;
};


class Order
{
public:
    OrderE type;
    uint16_t timeoutDs;
    Action * ptrActionPere;
    ptrFonction contreMesure;
    uint8_t nbFail;
    uint8_t nbMaxFail;
    union                         //Une union permet de dire que uniquement l'un de ces champs à un intéret
    {
        GOTO_S goTo;
        SPIN_S spin;
        SPINGOTO_S spinGoTo;
        SPINTO_S spinTo;
        FWD_S fwd;
        BWD_S bwd;
        STBY_S stby;
        SEND_S send;
    };

    Order();
};

class GOTO : public Order
{
public:
	GOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint16_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail,bool avoidance);
};

class SPIN : public Order
{
public:
	SPIN(uint8_t nerv, float thetaAim, uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class SPINTO : public Order
{
public:
	SPINTO(uint8_t nerv, float xAim, float yAim, uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class SPINGOTO : public Order
{
public:
	SPINGOTO(uint8_t nerv, float xAim, float yAim, uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail,bool avoidance);
};

class FWD : public Order
{
public:
	FWD(float acc, float v, uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class BWD : public Order
{
public:
	BWD(float acc, float v, uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class STBY : public Order
{
public:
    uint8_t nerv;
    MessageE unlockMessage;

	STBY(uint8_t nerv, MessageE unlockMessage, uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class SEND : public Order
{
public:
	SEND(MessageE message, uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class EMSTOP : public Order
{
public:
	EMSTOP(uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class SETX : public Order
{
public:
    SETX(uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class SETY : public Order
{
public:
    SETY(uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

#endif
