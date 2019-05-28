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

bool isInMainSquare(Vector vec,float marge){return (vec.y>0.60+marge && vec.y<2-marge && vec.x>marge && vec.x<3-marge);}

void Cerveau::computeEvitemment(float xObscl,float yObstcl)
{
    if (ptrRobot->ordresFifo.ptrFst()->type==OrderE::GOTO_E && ptrRobot->ordresFifo.ptrFst()->goTo.avoidance)
    {
        Vector obstcl=init_vector(xObscl,yObstcl);
        Vector delta=minus(obstcl,ptrRobot->posE.vec);
        float rObstcl=longueur(delta);
        float rAim=longueur(minus(ptrRobot->ordresFifo.ptrFst()->goTo.posAim,ptrRobot->posE.vec));
        if (rAim>rObstcl)
        {
            Vector orthogonal=rotate(mult(longueur(delta),delta));
            Vector evitemmentGauche=add (   obstcl,mult(DISTANCE_EVITEMMENT+2*RROBOT,orthogonal));
            Vector evitemmentDroite=add (   obstcl,mult(-1*(DISTANCE_EVITEMMENT+2*RROBOT),orthogonal));
            //Bordel ca fait du bien d'avoir de la place RAM
            Vector chosenOne=init_vector(0,0);
            if (isInMainSquare(evitemmentDroite,2*RROBOT) && isInMainSquare(evitemmentGauche,2*RROBOT))
            {
                if (longueur(minus(ptrRobot->ordresFifo.ptrFst()->goTo.posAim,evitemmentDroite))<longueur(minus(ptrRobot->ordresFifo.ptrFst()->goTo.posAim,evitemmentGauche)))
                {
                    chosenOne=evitemmentDroite;
                }
                else
                {
                    chosenOne=evitemmentGauche;
                }
            }
            else if(isInMainSquare(evitemmentDroite,2*RROBOT))
            {
                chosenOne=evitemmentDroite;
            }
            else if(isInMainSquare(evitemmentGauche,2*RROBOT))
            {
                chosenOne=evitemmentGauche;
            }

            if (isInMainSquare(evitemmentDroite,2*RROBOT) || isInMainSquare(evitemmentGauche,2*RROBOT))
            {
                Vector next=minus(ptrRobot->ordresFifo.ptrFst()->goTo.posAim,chosenOne);
                ptrRobot->ordresFifo.addHead(GOTO(STD,0.3,chosenOne.x,chosenOne.y,angle(next),false,50,nullptr,simpleTimeout,1,true));
                ptrRobot->ordresFifo.addHead(SPIN(RUSH,angle(orthogonal),30,nullptr,simpleTimeout,1));
                ptrRobot->ordresFifo.addHead(EMSTOP(20,nullptr,simpleTimeout,1));
                ptrRobot->pid.reload();
            }
            else
            {
                ptrRobot->ordresFifo.addHead(STBY(RUSH,MessageE::Evitemment_Clear,100,nullptr,jmeTire,1));
                ptrRobot->ordresFifo.addHead(EMSTOP(20,nullptr,simpleTimeout,1));
                ptrRobot->pid.reload();
            }
        }
    }

}       //TODO QUE FAIRE SI LE MESSAGE D EVITEMMENT PERSISTE

void Cerveau::abandonneCurrentAction()
{
  #ifdef STATE
  Serial.println("ABANDON ACTION");
  #endif

  DONE[currentActionIndex] = false;

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
  case Distribx3_1:
	  break;
  case Distribx3_2:
    break;
  case MonteRampe:
	  DONE[ActionE::PoseRampe] = true;
	  DONE[ActionE::DescendRampe] = true;
	  break;
  case PoseRampe:
	  break;
  case DescendRampe:
	  // On a raté l'actoin DescendRampe
	  // actionList[ActionE::DescendRampe].ordersList[1].goto.posAim = point A'
	  // buffer.clean
	  // DONE[ACtionE::DescendRampe] = false;
	  // LoadAction(DescendRampe)
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
  ptrRobot->ordresFifo.clean();
  addActionOrders();
}

void Cerveau::loadAction(ActionE actionType)
{
  switch (actionType)
  {
  case Chaos:
    actionList[ActionE::Chaos] = Action(actionType, 4);
    actionList[ActionE::Chaos].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    actionList[ActionE::Chaos].addGOTO(NERV, 0.4, 2.0, 1, 0, true, TMOUT,wiggle,2);
    actionList[ActionE::Chaos].addSPIN(STD, 1, 20,simpleTimeout,2);
    break;
  case Distribx6_1:
    actionList[actionType] = Action(actionType, 2);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case Distribx6_2:
    actionList[actionType] = Action(actionType, 2);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case Distribx6_3:
    actionList[actionType] = Action(actionType, 2);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case CoupDeCul:
    actionList[actionType] = Action(actionType, 0);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case Distribx3_1:
    actionList[actionType] = Action(actionType, 2);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case Distribx3_2:
    actionList[actionType] = Action(actionType, 1);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case MonteRampe:
    actionList[actionType] = Action(actionType, 0);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case PoseRampe:
    actionList[actionType] = Action(actionType, -3);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case DescendRampe:
    actionList[actionType] = Action(actionType, 0);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
	// Exemple :
	//actionList[actionType].addGOTO(Point A);
	//actionList[actionType].addGOTO(Point B);
	//actionList[actionType].addGOTO(Point C);
    break;
  case RecupBlueAcc:
    actionList[actionType] = Action(actionType, 1);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case PoseAcc:
    actionList[actionType] = Action(actionType, -9);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case RecupGoldAcc:
    actionList[actionType] = Action(actionType, 1);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case Balance:
    actionList[actionType] = Action(actionType, -2);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case PoseSol:
    actionList[actionType] = Action(actionType, 0);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  case CasseCouilles:
    actionList[actionType] = Action(actionType, 0);
    actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
    break;
  default:
    break;
  }
}

ActionE Cerveau::nextBestAction()
{
    // On load l'action suivante
    int i = (int) currentActionIndex;
    while (i < NB_ACTIONS-1)			// On prend l'action suivante non encore fini en partant de l'action actuelle dans la liste. -1 car il ne faut pas prendre CASSE COUILLES
    {
        if (!DONE[i])
            return (ActionE)i;
        i++;
    }
    //On refait un tour si on a rien trouve
	  i = 0;
	  while (i <  NB_ACTIONS-1)
	  {
  		  if (!DONE[i])
  			    return (ActionE)i;
  		  i++;
	  }

	  // On regarde si on a pas rate des palets
	  i = 0;
	  bool toutEstRamasse = true;
	  for (int i = 0; i < NB_ACTIONS-1; i++)
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
  		  while (i < NB_ACTIONS - 1)
  		  {
    			  if (!DONE[i])
    				  return (ActionE)i;
    			  i++;
  		  }
	  }

	  return ActionE::CasseCouilles; // Pas besoin mais au cas o� en cas de bug
}

void Cerveau::actuate()
{
    if (actionList[currentActionIndex].actionCompleted)
    {
		DONE[currentActionIndex] = true;

		currentActionIndex = nextBestAction(); // Choisir une nouvelle action
		addActionOrders();	// Ajoute ses ordres au buffer

    }
    //TODO Scrutage de Comm
    switch (ptrRobot->comm.lastMessage)
    {
        case (MessageE::Default):
        //Rien a faire
        break;
        case (MessageE::Evitemment):
            //ptrRobot->master->computeEvitemment(ptrRobot->comm.collisionX,ptrRobot->comm.collisionY);
            if (ptrRobot->pid.jeVeuxAvancer)
                ptrRobot->evitting=AnticolE::Front;
            else
                ptrRobot->evitting=AnticolE::Back;
            ptrRobot->ordresFifo.addHead(STBY(DYDM,Evitemment_Clear,50000,nullptr,simpleTimeout,1));
            ptrRobot->pid.reload();
            ptrRobot->comm.taken();
            break;
        case (MessageE::Evitemment_Clear):
            ptrRobot->evitting=AnticolE::No;
            break;
    }
}

void Cerveau::addActionOrders()
{
    actionList[currentActionIndex].addOrdersToBuffer(&(ptrRobot->ordresFifo));
}

Cerveau::Cerveau()
{
}

Cerveau::Cerveau(Robot * ptrRobot)
{
    this->ptrRobot = ptrRobot;

    for (int i = 0; i < NB_ACTIONS; i++)
    {
        DONE[i] = false;
        loadAction((ActionE)i);
    }

    currentActionIndex = ActionE::Chaos;		// On commence avec le Chaos et on ajoute les ordres au buffer
    addActionOrders();
}

Cerveau::~Cerveau()
{
}


void Cerveau::recallageBordure(bool recalleX, bool recalleY, Action * ptrPere)
{
    if (recalleX)
    {
        if(ptrRobot->posE.vec.x<1.5)
        {

            ptrRobot->ordresFifo.add(SPIN(STD,0.0,50,ptrPere,simpleTimeout,1));
            //ptrRobot->ordresFifo.add(BWD());  TODO implementer BWD. Ca recule jusqu'a ce que le bool pointé soit acitvé
        }
    }//TODO CONTINUER
}
