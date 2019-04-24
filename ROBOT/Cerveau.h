#ifndef CERVEAU_INCLUDED
#define CERVEAU_INCLUDED

#include "Arduino.h"
#include "Action.h"
#include "1_CONSTANT.h"

class Cerveau
{
public:
	Action actionList[12];
	bool DONE[12];

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
	void PoseSol();
	void addMonteRampe();
	void addPoseRampe();
	void addDescendRampe();

private:

};

#endif
