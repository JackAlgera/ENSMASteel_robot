#ifndef ACTION_INCLUDED
#define ACTION_INCLUDED

#include "1_CONSTANT.h"
#include "Order.h"
#include "Fifo.h"

class Action				            // Classe qui contiendra l'ensemble des actions predefinis (Chaos, Distribx6, Rampe etc)
{
public:
    ActionE type;			          // Type -> ActionE::Distribx6, ActionE::Chaos
    Order ordersList[TAILLEFIFO];		      // Chaque action (Chaos, Distribx6 etc) contient une liste d'ordres
    int nbrOrders;
    int currentOrderIndex;			// L'ordre actuel de l'action non complete
    bool actionCompleted;

    Action(ActionE type);
    Action();
    void addGOTO(uint8_t nerv, float fleche, float xAim, float yAim, float thetaAim, bool arret, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail,bool avoidance,bool relativ);
    void addGOTO(uint8_t nerv, float fleche, VectorE aim, bool arret, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail, bool avoidance, bool relativ);
    void addSPINGOTO(uint8_t nerv,float xAim, float yAim,uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail,bool avoidance,bool bwd);
    void addSPIN(uint8_t nerv, float thetaAim, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail,bool relativ);
    void addSPINTO(uint8_t nerv,float xAim,float yAim,uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    void addGO_UNTIL(bool arriere,uint8_t nerv,float distanceMax,MessageE unlockMessage, uint16_t timeoutDs, ptrFonction contreMesure,uint8_t nbMaxFail);
    void addSTBY(uint8_t nerv, MessageE unlockMessage, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    void addSEND(MessageE message, uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    void addEMSTOP(uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    void addSETX(float xValue,float theta,uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    void addSETY(float yValue,float theta,uint16_t timeoutDs, ptrFonction contreMesure, uint8_t nbMaxFail);
    Order * getCurrentOrder();
    void nextStep();

    void addOrdersToBuffer(Fifo* ordresFifo);

private:
    int currentOrderAdd = 0;	// Uniquement utilise lors de l'ajout des orders en lancant le robot
};

#endif
