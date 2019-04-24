#include "Cerveau.h"

// Remplissage des actions avec les Order
void Cerveau::addChaos()
{
	Action newAction(ActionE::Chaos, 3);	// Test creation de l'action Chaos avec 3 ordres
	newAction.addSPIN(STD, 1, 20);
	newAction.addSTBY(DYDM, "Tirt", 255);
	newAction.addSPIN(STD, 1, 20);

	actionList[ActionE::Chaos] = newAction;
}

void Cerveau::addDistribx6()
{
	Action newAction(ActionE::Distribx6, 3);	

	actionList[ActionE::Distribx6] = newAction;
}

void Cerveau::addDepart()
{
	Action newAction(ActionE::depart, 3);

	actionList[ActionE::depart] = newAction;
}

void Cerveau::addDistribx3()
{
	Action newAction(ActionE::Distribx3, 3);

	actionList[ActionE::Distribx3] = newAction;
}

void Cerveau::addRecupBleuAcc()
{
	Action newAction(ActionE::RecupBlueAcc, 3);

	actionList[ActionE::RecupBlueAcc] = newAction;
}

void Cerveau::addPoseAcc()
{
	Action newAction(ActionE::PoseAcc, 3);

	actionList[ActionE::PoseAcc] = newAction;
}

void Cerveau::addRecupGoldAcc()
{
	Action newAction(ActionE::RecupeGoldAcc, 3);

	actionList[ActionE::RecupeGoldAcc] = newAction;
}

void Cerveau::addBalance()
{
	Action newAction(ActionE::Balance, 3);

	actionList[ActionE::Balance] = newAction;
}

void Cerveau::addPoseSol()
{
	Action newAction(ActionE::PoseSol, 3);

	actionList[ActionE::PoseSol] = newAction;
}

void Cerveau::addMonteRampe()
{
	Action newAction(ActionE::MonteRampe, 3);

	actionList[ActionE::MonteRampe] = newAction;
}

void Cerveau::addPoseRampe()
{
	Action newAction(ActionE::PoseRampe, 3);

	actionList[ActionE::PoseRampe] = newAction;
}

void Cerveau::addDescendRampe()
{
	Action newAction(ActionE::DescendRamp, 3);

	actionList[ActionE::DescendRamp] = newAction;
}

Cerveau::Cerveau()
{
	for (int i = 0; i < 12; i++)
	{
		DONE[i] = false;
	}

	addChaos();
	addDistribx6();
	addDepart();
	addDistribx3();
	addRecupBleuAcc();
	addPoseAcc();
	addRecupGoldAcc();
	addBalance();
	addPoseSol();
	addMonteRampe();
	addDescendRampe();
	addPoseRampe();
}

Cerveau::~Cerveau()
{
}
