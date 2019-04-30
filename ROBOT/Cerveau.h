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
	Action actionList[12];		// Array qui contient l'ensemble des actions, qui contiennent chacun des ordres 
	bool DONE[12];				    // Array qui nous dit si un action est fini ou non
	ActionE currentAction;
	Fifo *ordresFifo;
	
	void supprimerAction(ActionE action);
	void choisirAction();		// TODO : a remplir
	ActionE nextBestAction(ActionE *list, int size);

	void finirAction();
	void addActionOrders();
	void finirOrdre();

	//void addNextOrder();		// Ajouter l'ordre suivant au buffer

	// Fonctions pour initialiser l'ensemble des actions 
	void addChaos();
	void addDistribx6();
	void addDepart();
	void addDistribx3();
	void addRecupBleuAcc();
	void addPoseAcc();
	void addRecupGoldAcc();
	void addBalance();
	void addPoseSol();
	void addMonteRampe();
	void addPoseRampe();
	void addDescendRampe();

	Cerveau();
	Cerveau(Fifo * ordresFifo);
	~Cerveau();
};

#endif
