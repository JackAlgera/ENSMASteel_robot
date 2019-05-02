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
    Action actionList[NBR_ACTIONS];		// Array qui contient l'ensemble des actions, qui contiennent chacun des ordres
    bool DONE[NBR_ACTIONS];				    // Array qui nous dit si un action est fini ou non
    ActionE currentActionIndex;
    Fifo *ordresFifo;

    void supprimerAction(ActionE action);
    void choisirAction();		// TODO : a remplir
    ActionE nextBestAction();

    void actuate();
    void addActionOrders();
    //void finirOrdre();

    //void addNextOrder();		// Ajouter l'ordre suivant au buffer
	
    Cerveau();
    Cerveau(Fifo * ordresFifo);
    ~Cerveau();
private:
    void addChaos();
  	void addDistribx6_1();
  	void addDistribx6_2();
  	void addDistribx6_3();
    void addCoupDeCul();
    void addDistribx3();
    void addRecupBlueAcc();
    void addPoseAcc();
    void addRecupGoldAcc();
    void addBalance();
    void addPoseSol();
    void addMonteRampe();
    void addPoseRampe();
	void addDescendRampe();
	void addCasseCouilles();
};

#endif
