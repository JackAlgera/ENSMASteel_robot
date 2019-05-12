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
    Robot *ptrRobot;
	bool isIdle = false;

    ActionE nextBestAction();

    void actuate();
    void addActionOrders();
    void abandonneCurrentAction();
    void loadAction(ActionE actionType);
    void computeEvitemment(float xObscl,float yObstcl);

    Cerveau();
    Cerveau(Robot * ptrRobot);
    ~Cerveau();
};

#endif
