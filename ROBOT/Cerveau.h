#ifndef CERVEAU_INCLUDED
#define CERVEAU_INCLUDED

#include "Arduino.h"
#include "Action.h"
#include "1_CONSTANT.h"

class Cerveau
{
public:
	Action actionList[12];		// Array qui contient l'ensemble des actions -> qui contiennent des ordres 
	bool DONE[12];				    // Array qui nous dit si un action est fini ou non

	Cerveau();
	~Cerveau();

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
};

#endif
