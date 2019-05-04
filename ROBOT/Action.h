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
    Order ordersList[TAILLEFIFO];		      // Chaque action (Chaos, Distribx6 etc) contient une liste d'ordres
    int nbrOrders;
    int currentOrderIndex;			// L'ordre actuel de l'action non complete
    bool actionCompleted;
	int nbrPalets;
	int nbrPalentsMax;

	Action(ActionE type, int nbrPalentsMax = 0);
    Action();
    void addGOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint8_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);  //abs(thetaAim-thetaIni)<=PI
    void addSPINGOTO(uint8_t nerv,float xAim, float yAim,uint8_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    void addSPIN(uint8_t nerv, float thetaAim, uint8_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    void addFWD(float acc, float v, uint8_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    void addBWD(float acc, float v, uint8_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    void addSTBY(uint8_t nerv, const char unlockMessage[], uint8_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    void addSEND(const char message[], uint8_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    void addEMSTOP(uint8_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    Order * getCurrentOrder();
    void nextStep();
	bool gotAllPalets();
	void addPalet(); // TODO : ajouter la fonction "addPalet" la ou on recupere des palets

    void addOrdersToBuffer(Fifo* ordresFifo);

private:
    int currentOrderAdd = 0;	// Uniquement utilise lors de l'ajout des orders en lancant le robot
};

#endif
