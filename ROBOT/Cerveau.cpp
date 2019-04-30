#include "Cerveau.h"


// bool thisIsVeryUgly = true;
// if(thisIsVeryUgly==true and true!=false)
// { 
//		Jack.Cry();
//		Jack.RollOnGround(appartment.floor);
//		Jack.CrySomeMore(100);  // en secondes
// }

// Liste des actions a prioriser pour chaque action, 
ActionE ChaosL[3] = { ActionE::Distribx6, ActionE::RecupBlueAcc };
ActionE Distribx6L[1] = { ActionE::Distribx3 };
ActionE Distribx3L[1] = { ActionE::CoupDeCul };
ActionE CoupDeCulL[1] = { ActionE::RecupBlueAcc };
ActionE RecupBlueAccL[1] = { ActionE::PoseAcc };
ActionE PoseAccL[1] = { ActionE::RecupGoldAcc };
ActionE RecupeGoldAccL[1] = { ActionE::Balance };
ActionE BalanceL[1] = { ActionE::PoseSol };
ActionE PoseSolL[1] = { ActionE::MonteRampe };
ActionE MonteRampeL[1] = { ActionE::PoseRampe };
ActionE PoseRampeL[1] = { ActionE::DescendRampe };
ActionE DescendRampL[1] = { ActionE::Chaos };

// Remplissage des actions avec les Order
Action newChaos()
{
	Action newAction(ActionE::Chaos, 4);	// Test creation de l'action Chaos avec 3 ordres
  newAction.addSEND("CHAO",10);
	newAction.addGOTO(NERV, 0.4, 2.0, 1, 0, true, TMOUT);
	newAction.addSPIN(STD, 1, 20);
	//newAction.addSPINGOTO(NERV, 1.5, 1, 50);
	
	return newAction;
}

Action newDistribx6()
{
	Action newAction(ActionE::Distribx6, 1);
  newAction.addSEND("DIx6",10);	

	return newAction;
}

Action newCoupDeCul()
{
	Action newAction(ActionE::CoupDeCul, 1);
  newAction.addSEND("CDCU",10);  

	return newAction;
}

Action newDistribx3()
{
	Action newAction(ActionE::Distribx3, 1);
  newAction.addSEND("DIx3",10);  

	return newAction;
}

Action newRecupBlueAcc()
{
	Action newAction(ActionE::RecupBlueAcc, 1);
  newAction.addSEND("RBLU",10); 

	return newAction;
}

Action newPoseAcc()
{
	Action newAction(ActionE::PoseAcc, 1);
  newAction.addSEND("PACC",10);  
	return newAction;
}
Action newRecupGoldAcc()
{
	Action newAction(ActionE::RecupGoldAcc, 1);
  newAction.addSEND("RGOL",10); 

	return newAction;
}

Action newBalance()
{
	Action newAction(ActionE::Balance, 1);
  newAction.addSEND("BALA",10);  

	return newAction;
}

Action newPoseSol()
{
	Action newAction(ActionE::PoseSol, 1);
  newAction.addSEND("PSOL",10);  

	return newAction;
}

Action newMonteRampe()
{
	Action newAction(ActionE::MonteRampe, 1);
 newAction.addSEND("MRMP",10); 

	return newAction;
}

Action newPoseRampe()
{
	Action newAction(ActionE::PoseRampe, 1);
  newAction.addSEND("PRMP",10);  

	return newAction;
}

Action newDescendRampe()
{
	Action newAction(ActionE::DescendRampe, 1);
  newAction.addSEND("DRMP",10); 

	return newAction;
}

void Cerveau::choisirAction()
{
	ActionE newAction = ActionE::Chaos;

	switch (currentAction)
	{
	case Chaos:
		nextBestAction(ChaosL, 3);
		break;
	case Distribx6:
		nextBestAction(Distribx6L, 1);
		break;
	case Distribx3:
		nextBestAction(Distribx3L, 1);
		break;
	case CoupDeCul:
		nextBestAction(CoupDeCulL, 1);
		break;
	case RecupBlueAcc:
		nextBestAction(RecupBlueAccL, 1);
		break;
	case PoseAcc:
		nextBestAction(PoseAccL, 1);
		break;
	case RecupGoldAcc:
		nextBestAction(RecupeGoldAccL, 1);
		break;
	case Balance:
		nextBestAction(BalanceL, 1);
		break;
	case PoseSol:
		nextBestAction(PoseSolL, 1);
		break;
	case MonteRampe:
		nextBestAction(MonteRampeL, 1);
		break;
	case PoseRampe:
		nextBestAction(PoseRampeL, 1);
		break;
	case DescendRampe:
		nextBestAction(DescendRampL, 1);
		break;
	default:
		break;
	}

	currentAction = newAction;
}

ActionE Cerveau::nextBestAction(ActionE *list, int size)
{
	// On prend la premiere action disponible en prioritisant les actions dans l'ordre de la liste 
	int i = 0;
	while (i < size)
	{
		if (!DONE[list[i]])
			return list[i];

		i++;
	}
	// Sinon on prend la premiere action non complete
	i = 0;
	while (i < NBR_ACTIONS)
	{
		if (!DONE[i])
			return (ActionE)i;
		i++;
	}
	return ActionE::Chaos; // Ici il faudrait faire une action "casse les couilles de l'adversaire"
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
	// J'ai commenc� � le faire, je le garde au cas ou 
	/*
	if (!actionList[currentAction].finirOrder()) // On finit l'ordre et si l'action n'est pas encore fini
	{
		addNextOrder();							
	}
	else											// Sinon l'action est termin�
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

	actionList[ActionE::Chaos]=newChaos();
	actionList[ActionE::Distribx6]=newDistribx6();
	actionList[ActionE::CoupDeCul]=newCoupDeCul();
	actionList[ActionE::Distribx3]=newDistribx3();
	actionList[ActionE::RecupBlueAcc]=newRecupBlueAcc();
	actionList[ActionE::PoseAcc]=newPoseAcc();
	actionList[ActionE::RecupGoldAcc]=newRecupGoldAcc();
	actionList[ActionE::Balance]=newBalance();
	actionList[ActionE::PoseSol]=newPoseSol();
	actionList[ActionE::MonteRampe]=newMonteRampe();
	actionList[ActionE::DescendRampe]=newDescendRampe();
	actionList[ActionE::PoseRampe]=newPoseRampe();

	currentAction = ActionE::Chaos;		// On commence avec le Chaos et on ajoute les ordres au buffer
	addActionOrders();
}

Cerveau::~Cerveau()
{
}
