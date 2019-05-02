#include "Cerveau.h"


// bool thisIsVeryUgly = true;
// if(thisIsVeryUgly==true and true!=false)
// {
//		Jack.Cry();
//		Jack.RollOnGround(appartment.floor);
//		Jack.CrySomeMore(100);  // en secondes
// }

// Remplissage des actions avec les Order
void Cerveau::addChaos()
{
    actionList[ActionE::Chaos] = Action(ActionE::Chaos);
    actionList[ActionE::Chaos].addSEND("CHAO",10);
    actionList[ActionE::Chaos].addGOTO(NERV, 0.4, 2.0, 1, 0, true, TMOUT);
    actionList[ActionE::Chaos].addSPIN(STD, 1, 20);
}

void Cerveau::addDistribx6_1()
{
    actionList[ActionE::Distribx6_1] = Action(ActionE::Distribx6_1);
    actionList[ActionE::Distribx6_1].addSEND("Dx61",10);
}

void Cerveau::addDistribx6_2()
{
	actionList[ActionE::Distribx6_2] = Action(ActionE::Distribx6_2);
	actionList[ActionE::Distribx6_2].addSEND("Dx62", 10);
}

void Cerveau::addDistribx6_3()
{
	actionList[ActionE::Distribx6_3] = Action(ActionE::Distribx6_3);
	actionList[ActionE::Distribx6_3].addSEND("Dx63", 10);
}

void Cerveau::addCoupDeCul()
{
    actionList[ActionE::CoupDeCul] = Action(ActionE::CoupDeCul);
    actionList[ActionE::CoupDeCul].addSEND("CDCU",10);
}

void Cerveau::addDistribx3()
{
    actionList[ActionE::Distribx3] = Action(ActionE::Distribx3);
    actionList[ActionE::Distribx3].addSEND("DIx3",10);
}

void Cerveau::addRecupBlueAcc()
{
    actionList[ActionE::RecupBlueAcc] = Action(ActionE::RecupBlueAcc);
    actionList[ActionE::RecupBlueAcc].addSEND("RBLU",10);
}

void Cerveau::addPoseAcc()
{
    actionList[ActionE::PoseAcc] = Action(ActionE::PoseAcc);
    actionList[ActionE::PoseAcc].addSEND("PACC",10);
}
void Cerveau::addRecupGoldAcc()
{
    actionList[ActionE::RecupGoldAcc] = Action(ActionE::RecupGoldAcc);
    actionList[ActionE::RecupGoldAcc].addSEND("RGOL",10);
}

void Cerveau::addBalance()
{
    actionList[ActionE::Balance] = Action(ActionE::Balance);
    actionList[ActionE::Balance].addSEND("BALA",10);
}

void Cerveau::addPoseSol()
{
    actionList[ActionE::PoseSol] = Action(ActionE::PoseSol);
    actionList[ActionE::PoseSol].addSEND("PSOL",10);
}

void Cerveau::addMonteRampe()
{
    actionList[ActionE::MonteRampe] = Action(ActionE::MonteRampe);
    actionList[ActionE::MonteRampe].addSEND("MRMP",10);
}

void Cerveau::addPoseRampe()
{
    actionList[ActionE::PoseRampe] = Action(ActionE::PoseRampe);
    actionList[ActionE::PoseRampe].addSEND("PRMP",10);
}

void Cerveau::addDescendRampe()
{
    actionList[ActionE::DescendRampe] = Action(ActionE::DescendRampe);
    actionList[ActionE::DescendRampe].addSEND("DRMP",10);
}

void Cerveau::addCasseCouilles()
{
}

void Cerveau::choisirAction()
{
    ActionE newAction = ActionE::Chaos;

	switch (currentActionIndex)
	{
	case Chaos:
		newAction = nextBestAction();
		break;
	case Distribx6_1:
		newAction = nextBestAction();
		break;
	case Distribx6_2:
		newAction = nextBestAction();
		break;
	case Distribx6_3:
		newAction = nextBestAction();
		break;
	case CoupDeCul:
		newAction = nextBestAction();
		break;
	case Distribx3:
		newAction = nextBestAction();
		break;
	case MonteRampe:
		newAction = nextBestAction();
		break;
	case PoseRampe:
		newAction = nextBestAction();
		break;
	case DescendRampe:
		newAction = nextBestAction();
		break;
	case RecupBlueAcc:
		newAction = nextBestAction();
		break;
	case PoseAcc:
		newAction = nextBestAction();
		break;
	case RecupGoldAcc:
		newAction = nextBestAction();
		break;
	case Balance:
		newAction = nextBestAction();
		break;
	case PoseSol:
		newAction = nextBestAction();
		break;
	case CasseCouilles:
		newAction = nextBestAction();
		break;
	default:
		break;
	}

    currentActionIndex = newAction;
}

ActionE Cerveau::nextBestAction()
{
    // Sinon on prend la premiere action non complete
    int i = (int) currentActionIndex;
    while (i < NBR_ACTIONS)			// On prend l'action suivante non encore fini en partant de l'action actuelle dans la liste
    {
        if (!DONE[i])
            return (ActionE)i;
        i++;
    }
	i = 0;
	while (i < (int)currentActionIndex)	
	{
		if (!DONE[i])
			return (ActionE)i;
		i++;
	}

    return ActionE::Chaos; // Ici il faudrait faire une action "casse les couilles de l'adversaire" // +1 pouce bleu
}

void Cerveau::actuate()
{
    if (actionList[currentActionIndex].actionCompleted)
    {
        DONE[currentActionIndex] = true;

        choisirAction();	// Choisir une nouvelle action
        addActionOrders();	// Ajoute ses ordres au buffer
    }
}

void Cerveau::addActionOrders()
{
    actionList[currentActionIndex].addOrdersToBuffer(ordresFifo);
}

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

    for (int i = 0; i < NBR_ACTIONS; i++)
    {
        DONE[i] = false;
    }

    //actionList[ActionE::Chaos]=newChaos();
    addChaos();
    addDistribx6_1();
    addDistribx6_2();
    addDistribx6_3();
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

    currentActionIndex = ActionE::Chaos;		// On commence avec le Chaos et on ajoute les ordres au buffer
    addActionOrders();
}

Cerveau::~Cerveau()
{
}
