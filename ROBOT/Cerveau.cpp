#include "Cerveau.h"

// Remplissage des actions avec les Order
void Cerveau::addChaos()
{
	Action newAction(ActionE::Chaos, 3);	// Test creation de l'action Chaos avec 3 ordres

	newAction.addGOTO(NERV, 0.4, 2.0, 1, 0, true, TMOUT);
	newAction.addSPIN(STD, 1, 20);
	newAction.addSPINGOTO(NERV, 0, 0, 50);
	
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

void Cerveau::choisirAction()
{
	ActionE newAction = ActionE::Chaos;

	switch (currentAction)
	{
	case Chaos:
		// Prioriser distribx6
		break;
	case Distribx6:
		// Prioriser distribx3
		break;
	case Distribx3:
		break;
	case depart:
		break;
	case RecupBlueAcc:
		break;
	case PoseAcc:
		break;
	case RecupeGoldAcc:
		break;
	case Balance:
		break;
	case PoseSol:
		break;
	case MonteRampe:
		break;
	case PoseRampe:
		break;
	case DescendRamp:
		break;
	default:		// Sinon on choisit l'action suivant de la façon suivante, pas obligé de faire un test pour chaque cas
		

		break;
	}

	currentAction = newAction;
}

void Cerveau::finirAction()
{
	DONE[currentAction] = true;

	choisirAction();	// Choisir une nouvelle action 
	addActionOrders();	// Ajoute ses ordres au buffer
}

void Cerveau::addActionOrders()
{
	actionList[currentAction].addOrdersToBuffer(ordresFifo);
}

void Cerveau::finirOrdre()
{
	ordresFifo->pop();	// Pop le fifo
	if (actionList[currentAction].finirOrder()) // Si l'action actuelle est termine
	{
		finirAction();
	}

	// Si on n'ajoute pas l'ensemble des ordres au buffer lors d'un changement d'acion, donc en ajoutant l'ordre par ordre
	// J'ai commencé à le faire, je le garde au cas ou 
	/*
	if (!actionList[currentAction].finirOrder()) // On finit l'ordre et si l'action n'est pas encore fini
	{
		addNextOrder();							
	}
	else											// Sinon l'action est terminé
	{
		DONE[currentAction] = true;
		choisirAction();							// On choisit l'action suivant
	}
	*/
}

/*
void Cerveau::addNextOrder()						// Ajouter l'ordre suivant au buffer
{
	ordresFifo->add(*actionList[currentAction].getCurrentOrder());
}
*/

void Cerveau::supprimerAction(ActionE action)
{
	DONE[action] = true;

	choisirAction();	// Choisir l'action suivant 
	addActionOrders();	// Ajouter ordres au buffer
}

Cerveau::Cerveau()
{
}

Cerveau::Cerveau(Fifo * ordresFifo)
{
	this->ordresFifo = ordresFifo;

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

	currentAction = ActionE::Chaos;		// On commence avec le Chaos et on ajoute les ordres au buffer
	addActionOrders();
}

Cerveau::~Cerveau()
{
}
