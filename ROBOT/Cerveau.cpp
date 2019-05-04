#include "Cerveau.h"
#include "Robot.h"
#include "ContreMesure.h"

// bool thisIsVeryUgly = true;
// if(thisIsVeryUgly==true and true!=false)
// {
//		Jack.Cry();
//		Jack.RollOnGround(appartment.floor);
//		Jack.CrySomeMore(100);  // en secondes
// }

void Cerveau::abandonneCurrentAction()
{
  #ifdef STATE
  Serial.println("ABANDON ACTION");
  #endif

  DONE[currentActionIndex] = true;

  switch (currentActionIndex)
  {
  case Chaos:
	  break;
  case Distribx6_1:
	  break;
  case Distribx6_2:
	  break;
  case Distribx6_3:
	  break;
  case CoupDeCul:
	  break;
  case Distribx3:
	  break;
  case MonteRampe:
	  DONE[ActionE::PoseRampe] = true;
	  DONE[ActionE::DescendRampe] = true;
	  break;
  case PoseRampe:
	  break;
  case DescendRampe:
	  break;
  case RecupBlueAcc:
	  break;
  case PoseAcc:
	  break;
  case RecupGoldAcc:
	  break;
  case Balance:
	  break;
  case PoseSol:
	  break;
  case CasseCouilles:
	  break;
  default:
	  break;
  }

  currentActionIndex = nextBestAction();
  addActionOrders();
}

// Remplissage des actions avec les Order
void Cerveau::addChaos()
{
    actionList[ActionE::Chaos] = Action(ActionE::Chaos, 4);
    actionList[ActionE::Chaos].addSEND("CHAO",10,simpleTimeout,2);
    actionList[ActionE::Chaos].addGOTO(NERV, 0.4, 2.0, 1, 0, true, TMOUT,wiggle,2);
    actionList[ActionE::Chaos].addSPIN(STD, 1, 20,simpleTimeout,2);
}

void Cerveau::addDistribx6_1()
{
    actionList[ActionE::Distribx6_1] = Action(ActionE::Distribx6_1, 2);
    actionList[ActionE::Distribx6_1].addSEND("Dx61",10,simpleTimeout,2);
}

void Cerveau::addDistribx6_2()
{
	actionList[ActionE::Distribx6_2] = Action(ActionE::Distribx6_2, 2);
	actionList[ActionE::Distribx6_2].addSEND("Dx62", 10,simpleTimeout,2);
}

void Cerveau::addDistribx6_3()
{
	actionList[ActionE::Distribx6_3] = Action(ActionE::Distribx6_3, 2);
	actionList[ActionE::Distribx6_3].addSEND("Dx63", 10,simpleTimeout,2);
}

void Cerveau::addCoupDeCul()
{
    actionList[ActionE::CoupDeCul] = Action(ActionE::CoupDeCul);
    actionList[ActionE::CoupDeCul].addSEND("CDCU",10,simpleTimeout,2);
}

void Cerveau::addDistribx3()
{
    actionList[ActionE::Distribx3] = Action(ActionE::Distribx3, 3);
    actionList[ActionE::Distribx3].addSEND("DIx3",10,simpleTimeout,2);
}

void Cerveau::addRecupBlueAcc()
{
    actionList[ActionE::RecupBlueAcc] = Action(ActionE::RecupBlueAcc, 1);
    actionList[ActionE::RecupBlueAcc].addSEND("RBLU",10,simpleTimeout,2);
}

void Cerveau::addPoseAcc()
{
    actionList[ActionE::PoseAcc] = Action(ActionE::PoseAcc);
    actionList[ActionE::PoseAcc].addSEND("PACC",10,simpleTimeout,2);
}
void Cerveau::addRecupGoldAcc()
{
    actionList[ActionE::RecupGoldAcc] = Action(ActionE::RecupGoldAcc, 1);
    actionList[ActionE::RecupGoldAcc].addSEND("RGOL",10,simpleTimeout,2);
}

void Cerveau::addBalance()
{
    actionList[ActionE::Balance] = Action(ActionE::Balance);
    actionList[ActionE::Balance].addSEND("BALA",10,simpleTimeout,2);
}

void Cerveau::addPoseSol()
{
    actionList[ActionE::PoseSol] = Action(ActionE::PoseSol);
    actionList[ActionE::PoseSol].addSEND("PSOL",10,simpleTimeout,2);
}

void Cerveau::addMonteRampe()
{
    actionList[ActionE::MonteRampe] = Action(ActionE::MonteRampe);
    actionList[ActionE::MonteRampe].addSEND("MRMP",10,simpleTimeout,2);
}

void Cerveau::addPoseRampe()
{
    actionList[ActionE::PoseRampe] = Action(ActionE::PoseRampe);
    actionList[ActionE::PoseRampe].addSEND("PRMP",10,simpleTimeout,2);
}

void Cerveau::addDescendRampe()
{
    actionList[ActionE::DescendRampe] = Action(ActionE::DescendRampe);
    actionList[ActionE::DescendRampe].addSEND("DRMP",10,simpleTimeout,2);
}

void Cerveau::addCasseCouilles()
{
    actionList[ActionE::CasseCouilles] = Action(ActionE::CasseCouilles);
    actionList[ActionE::CasseCouilles].addSEND("BALL",10,simpleTimeout,2);
}

void Cerveau::choisirAction()
{
    ActionE newAction = ActionE::Chaos;

	switch (currentActionIndex)
	{
	case Chaos:
		break;
	case Distribx6_1:
		break;
	case Distribx6_2:
		break;
	case Distribx6_3:
		break;
	case CoupDeCul:
		break;
	case Distribx3:
		break;
	case MonteRampe:
		break;
	case PoseRampe:
		break;
	case DescendRampe:
		break;
	case RecupBlueAcc:
		break;
	case PoseAcc:
		break;
	case RecupGoldAcc:
		break;
	case Balance:
		break;
	case PoseSol:
		break;
	case CasseCouilles:
		break;
	default:
		break;
	}

	newAction = nextBestAction();
    currentActionIndex = newAction;
}

ActionE Cerveau::nextBestAction()
{
    // On load l'action suivante
    int i = (int) currentActionIndex;
    while (i < NBR_ACTIONS-1)			// On prend l'action suivante non encore fini en partant de l'action actuelle dans la liste. -1 car il ne faut pas prendre CASSE COUILLES
    {
        if (!DONE[i])
            return (ActionE)i;
        i++;
    }
    //On refait un tour si on a rien trouve
	  i = 0;
	  while (i <  NBR_ACTIONS-1)
	  {
		  if (!DONE[i])
			  return (ActionE)i;
		  i++;
	  }

	  // On regarde si on a pas rate des palets
	  i = 0;
	  bool toutEstRamasse = true;
	  for (int i = 0; i < NBR_ACTIONS-1; i++)
	  {
		  if (!actionList[i].gotAllPalets())
		  {
			  DONE[i] = false;
			  toutEstRamasse = false;
		  }
	  }
	  
	  if (toutEstRamasse)  // si on a ramasse tout les palets
	  {
		  return ActionE::CasseCouilles; //On a fini, on va alors accidentelement faire chier l'adversaire
	  }
	  else                 // Sinon on recup les palets manquant 
	  {
		  DONE[ActionE::PoseSol] = false; // On refait l'action PoseSol pour poser les palets qu'on vient de recuperer et on choisit une autre action
		  i = 0;
		  while (i < NBR_ACTIONS - 1)
		  {
			  if (!DONE[i])
				  return (ActionE)i;
			  i++;
		  }
	  }

	  return ActionE::CasseCouilles; // Pas besoin mais au cas où en cas de bug
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
    actionList[currentActionIndex].addOrdersToBuffer(ptrFifo);
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

Cerveau::Cerveau(Fifo * ptrFifo)
{
    this->ptrFifo = ptrFifo;

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
    addCasseCouilles();

    currentActionIndex = ActionE::Chaos;		// On commence avec le Chaos et on ajoute les ordres au buffer
    addActionOrders();
}

Cerveau::~Cerveau()
{
}
