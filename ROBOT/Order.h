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
    bool arret,avoidance,relativ;
};

struct SPIN_S
{
    uint8_t nerv;
    float thetaAim;       //angle cible, acceleration et deceleration angulaire (theta point point)
    bool relativ;
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
    bool avoidance,bwd;
};

struct GO_UNTIL_S
{
    bool arriere;
    uint8_t nerv;
    float distanceMax;
    MessageE unlockMessage;
};

struct STBY_S
{
    uint8_t nerv;
    MessageE unlockMessage;
};

struct SETX_S
{
    float xValue,theta;
};

struct SETY_S
{
    float yValue,theta;
};

struct SEND_S
{
    MessageE message;
};


class Order
{
public:
    OrderE type;
    float errorIntegralLin,errorIntegralAng;
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
        GO_UNTIL_S go_until;
        STBY_S stby;
        SEND_S send;
        SETX_S setx;
        SETY_S sety;
    };

    Order();
};

class GOTO : public Order
{
public:
	GOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint16_t timeoutDs, Action * ptrActionPere, ptrFonction contreMesure, uint8_t nbMaxFail,bool avoidance,bool relativ=false);
};

class SPIN : public Order
{
public:
	SPIN(uint8_t nerv, float thetaAim, uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail,bool relativ=false);
};

class SPINTO : public Order
{
public:
	SPINTO(uint8_t nerv, float xAim, float yAim, uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class SPINGOTO : public Order
{
public:
	SPINGOTO(uint8_t nerv, float xAim, float yAim, uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail,bool avoidance,bool bwd);
};

class GO_UNTIL : public Order
{
public:
    GO_UNTIL(bool arriere,uint8_t nerv,float distanceMax,MessageE unlockMessage, uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};


class STBY : public Order
{
public:
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
    SETX(float xValue,float theta,uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

class SETY : public Order
{
public:
    SETY(float yValue,float theta,uint16_t timeoutDs, Action * ptrActionPere,ptrFonction contreMesure,uint8_t nbMaxFail);
};

#endif
