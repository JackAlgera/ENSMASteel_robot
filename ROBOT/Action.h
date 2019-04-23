#ifndef ACTION_INCLUDED
#define ACTION_INCLUDED

#include "CONSTANT.h"
#include "Order.h"
#include "Arduino.h"

enum Actions : uint8_t
{
  Chaos, Distribx6, depart, Distribx3, Rampe,               // On aura Chaos = 0, Distribx6 = 1 etc..
  RecupBlueAcc, PoseAcc, RecupeGoldAcc, Balance, PoseSol    // Actions::Chaos pour y acceder -> exemple : if(!DONE[Actions::Chaos]) {...}
};   

class Action				// Classe qui contiendra l'ensemble des actions predefinis (Chaos, Distribx6, Rampe etc)
{
  public:
  	Actions type;			    // Type -> Actions::Distribx6, Actions::Chaos
  	Order_new * ordersList;		// Chaque action (Chaos, Distribx6 etc) contient une liste d'ordres
  	int nbrOrders;
  	int currentOrder;
  	bool actionCompleted;
  
  	Action(Actions type, int nbrOrders);
  	void addGoto(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint8_t timeoutDs);  //abs(thetaAim-thetaIni)<=PI
  	void addSpin(uint8_t nerv, float thetaAim, uint8_t timeoutDs);
  	void addFWD(float acc, float v, uint8_t timeoutDs);
  	void addBWD(float acc, float v, uint8_t timeoutDs);
  	void addSTBY(uint8_t nerv, char* unlockMessage, uint8_t timeout);
  	Order_new * getCurrentOrder();
  
  private:
  	int currentOrderAdd = 0;	// Uniquement utilise lors de l'ajout des orders en lancant le robot
};

#endif
