#ifndef ORDER_INCLUDED
#define ORDER_INCLUDED

#include "1_CONSTANT.h"
#include "Vector.h"
#include "Arduino.h"

class Order_new
{
  public:
  	OrderE type;
  	uint8_t timeOutDS;
  	Order_new();
};

class GoTo : public Order_new
{
  public:
  	uint8_t nerv;
  	float fleche;         //La fl�che est sans unit�. Elle indique de combien le robot peut d'�loigner du trajet le plus court
  	Vector posAim;        //Position cibe
  	float thetaAim;       //Deceleration, angle cible, vitesse cible
  	bool arret;
  
  	GoTo(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint8_t timeoutDS) //abs(thetaAim-thetaIni)<=PI
    {
      this->type      = OrderE::GoTo_E;
      this->timeOutDS = timeoutDS;
    
      this->nerv      = nerv;
      this->fleche    = fleche;
      this->posAim.x  = xAim;
      this->posAim.y  = yAim;
      this->thetaAim  = thetaAim;
      this->arret     = arret;
    }
};

class Spin : public Order_new
{
  public:
  	uint8_t nerv;
  	float thetaAim;       //angle cible, acceleration et deceleration angulaire (theta point point)
  
  	Spin(uint8_t nerv, float thetaAim, uint8_t timeoutDS)
    {
      this->type      = OrderE::Spin_E;
      this->timeOutDS = timeoutDS;
    
      this->nerv      = nerv;
      this->thetaAim  = thetaAim;
    }
};

class FWD : public Order_new
{
  public:
  	float acc, v;          //Acceleration, vitesse max, temps maximum pass� sur cette instruction
  
  	FWD(float acc, float v, uint8_t timeoutDS)
    {
      this->type      = OrderE::FWD_E;
      this->timeOutDS = timeoutDS;
    
      this->acc       = acc;
      this->v         = v;
    }
};

class BWD : public Order_new
{
  public:
  	float acc, v;
  
  	BWD(float acc, float v, uint8_t timeoutDS)
    {
      this->type      = OrderE::BWD_E;
      this->timeOutDS = timeoutDS;
    
      this->acc       = acc;
      this->v         = v;
    }
};

class STBY_new : public Order_new
{
  public:
  	uint8_t nerv;
  	char unlockMessage[4];
  
  	STBY_new(uint8_t nerv, const char unlockMessage[], uint8_t timeoutDS)
    {
      this->type          = OrderE::STBY_E;
      this->timeOutDS     = timeoutDS;
    
      this->nerv          = nerv;
	  this->unlockMessage[0] = unlockMessage[0];
	  this->unlockMessage[1] = unlockMessage[1];
	  this->unlockMessage[2] = unlockMessage[2];
	  this->unlockMessage[3] = unlockMessage[3];
    }
};

#endif
