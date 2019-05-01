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
void Cerveau::addChaos()
{
	actionList[ActionE::Chaos].set(ActionE::Chaos,3);
	actionList[ActionE::Chaos].addSEND("CHAO",10);
	actionList[ActionE::Chaos].addGOTO(NERV, 0.4, 2.0, 1, 0, true, TMOUT);
	actionList[ActionE::Chaos].addSPIN(STD, 1, 20);
}

void Cerveau::addDistribx6()
{
  actionList[ActionE::Distribx6].set(ActionE::Distribx6, 1);
  actionList[ActionE::Distribx6].addSEND("DIx6",10);	
}

void Cerveau::addCoupDeCul()
{
	actionList[ActionE::CoupDeCul].set(ActionE::CoupDeCul, 1);
  actionList[ActionE::CoupDeCul].addSEND("CDCU",10);  
}

void Cerveau::addDistribx3()
{
	actionList[ActionE::Distribx3].set(ActionE::Distribx3, 1);
  actionList[ActionE::Distribx3].addSEND("DIx3",10); 
}

void Cerveau::addRecupBlueAcc()
{
	actionList[ActionE::RecupBlueAcc].set(ActionE::RecupBlueAcc, 1);
  actionList[ActionE::RecupBlueAcc].addSEND("RBLU",10); 
}

void Cerveau::addPoseAcc()
{
  actionList[ActionE::PoseAcc].set(ActionE::PoseAcc, 1);
  actionList[ActionE::PoseAcc].addSEND("PACC",10); 
}
void Cerveau::addRecupGoldAcc()
{
  actionList[ActionE::RecupGoldAcc].set(ActionE::RecupGoldAcc, 1);
  actionList[ActionE::RecupGoldAcc].addSEND("RGOL",10); 
}

void Cerveau::addBalance()
{
	actionList[ActionE::Balance].set(ActionE::Balance, 1);
  actionList[ActionE::Balance].addSEND("BALA",10);  
}

void Cerveau::addPoseSol()
{
  actionList[ActionE::PoseSol].set(ActionE::PoseSol, 1);
  actionList[ActionE::PoseSol].addSEND("PSOL",10);  
}

void Cerveau::addMonteRampe()
{
  actionList[ActionE::MonteRampe].set(ActionE::MonteRampe, 1);
  actionList[ActionE::MonteRampe].addSEND("MRMP",10); 
}

void Cerveau::addPoseRampe()
{
  actionList[ActionE::PoseRampe].set(ActionE::PoseRampe, 1);
  actionList[ActionE::PoseRampe].addSEND("PRMP",10); 
}

void Cerveau::addDescendRampe()
{
  actionList[ActionE::DescendRampe].set(ActionE::DescendRampe, 1);
  actionList[ActionE::DescendRampe].addSEND("DRMP",10); 
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
	// On prend la premiere action disponible en priorisant les actions dans l'ordre de la liste 
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

void Cerveau::actuate()
{
	Serial.print("&actionList[ActionE::Chaos]= ");Serial.println((int)&actionList[ActionE::Chaos]);
	//Serial.println(actionList[currentAction].currentOrder);
	if (actionList[currentAction].actionCompleted)
  {
	  DONE[currentAction] = true;

	  choisirAction();	// Choisir une nouvelle action 
	  addActionOrders();	// Ajoute ses ordres au buffer
  }
}

void Cerveau::addActionOrders()
{
	actionList[currentAction].addOrdersToBuffer(ordresFifo);
}

/*
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
	
}
  */
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

	//actionList[ActionE::Chaos]=newChaos();
	addChaos();
  addDistribx6();
  addCoupDeCul();
  addDistribx3();
  addRecupBlueAcc();
  addPoseAcc();
  addRecupGoldAcc();
  addBalance();
  addPoseSol();
  addMonteRampe();
  addPoseRampe();
  addDescendRampe();

	currentAction = ActionE::Chaos;		// On commence avec le Chaos et on ajoute les ordres au buffer
	addActionOrders();
}

Cerveau::~Cerveau()
{
}
