#ifndef CERVEAU_INCLUDED
#define CERVEAU_INCLUDED

#include "Arduino.h"
#include "Action.h"
#include "1_CONSTANT.h"
#include "Fifo.h"
#include "PID.h"



class Cerveau
{
public:
    Action actionList[NB_ACTIONS];		// Array qui contient l'ensemble des actions, qui contiennent chacun des ordres
    bool DONE[NB_ACTIONS];				    // Array qui nous dit si un action est fini ou non
    ActionE currentActionIndex;
    Fifo *ptrFifo;

    ActionE nextBestAction();

    void actuate();
    void addActionOrders();
    void abandonneCurrentAction();
    void loadAction(ActionE actionType);
	
    Cerveau();
    Cerveau(Fifo * ptrFifo);
    ~Cerveau();
};

#endif
