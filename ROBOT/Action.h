#ifndef ACTION_INCLUDED
#define ACTION_INCLUDED

#include "1_CONSTANT.h"
#include "Order.h"
#include "Arduino.h"
#include "Fifo.h"

class Action				            // Classe qui contiendra l'ensemble des actions predefinis (Chaos, Distribx6, Rampe etc)
{
  public:
  	ActionE type;			          // Type -> ActionE::Distribx6, ActionE::Chaos
  	Order * ordersList;		      // Chaque action (Chaos, Distribx6 etc) contient une liste d'ordres
  	int nbrOrders;
  	int currentOrderIndex;			// L'ordre actuel de l'action non complete
	  int currentBufferOrderIndex;		// Dernier ordre ajout� au buffer
  	bool actionCompleted;
  
  	Action(ActionE type, int nbrOrders);
	  Action();
    void set(ActionE type, int nbrOrders);
  	void addGOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint8_t timeoutDs,Action * papa);  //abs(thetaAim-thetaIni)<=PI
    void addSPINGOTO(uint8_t nerv,float xAim, float yAim,uint8_t timeoutDs,Action * papa);
  	void addSPIN(uint8_t nerv, float thetaAim, uint8_t timeoutDs,Action * papa);
  	void addFWD(float acc, float v, uint8_t timeoutDs,Action * papa);
  	void addBWD(float acc, float v, uint8_t timeoutDs,Action * papa);
  	void addSTBY(uint8_t nerv, const char unlockMessage[], uint8_t timeout,Action * papa);
	  void addSEND(const char message[], uint8_t timeoutDs,Action * papa);
	  void addEMSTOP(uint8_t timeoutDs,Action * papa);
  	Order * getCurrentOrder();
	  void nextStep();
  
	  void addOrdersToBuffer(Fifo* ordresFifo, bool reAdd = false);

  private:
  	int currentOrderAdd = 0;	// Uniquement utilise lors de l'ajout des orders en lancant le robot
};

#endif
