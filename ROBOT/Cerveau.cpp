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

bool isInMainSquare(Vector vec,float marge)
{
    return (vec.y>0.60+marge && vec.y<2-marge && vec.x>marge && vec.x<3-marge);
}

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


void Cerveau::loadAction(ActionE actionType)
{
    VectorE startPoint1,startPoint2,chaosPoint1,chaosPoint2,releasePoint;
    switch (actionType)
    {
    case Start:
        while (true)
        {
            if(ptrRobot->contacteurGauche.isJustPressed())
            {
                coteViolet=true;
                ptrRobot->comm.send(MessageE::Violet);
                ptrRobot->posE.vec.x=2.7;
                ptrRobot->posE.vec.y=1.4;
                ptrRobot->posE.theta=PI;
                #ifdef STATE
                Serial.print("Cote Violet");
                #endif // STATE
                break;
            }
            if(ptrRobot->contacteurDroite.isJustPressed())
            {
                coteViolet=false;
                ptrRobot->comm.send(MessageE::Jaune);
                ptrRobot->posE.vec.x=0.3;
                ptrRobot->posE.vec.y=1.4;
                ptrRobot->posE.theta=0.0;
                #ifdef STATE
                Serial.print("Cote Jaune");
                #endif // STATE
                break;
            }
            delay(1);
        }

        actionList[ActionE::Start] = Action(actionType);
        actionList[ActionE::Start].addSTBY(DYDM,Impossible,1,normalTimeout,1);
        actionList[ActionE::Start].addSEND(MessageE::Pince_Retracted,10,simpleTimeout,1);
        //Calage X
        actionList[ActionE::Start].addGO_UNTIL(true,RECALLE,0.3,MessageE::Calle,100,simpleTimeout,1);
        actionList[ActionE::Start].addSETX((coteViolet)?(3.0-HROBOT):(HROBOT),(coteViolet)?(PI):(0.0),10,simpleTimeout,1);
        actionList[ActionE::Start].addSTBY(OFF,Impossible,3,normalTimeout,1);
        actionList[ActionE::Start].addGOTO(STD,0.1,0.10,0.0,0.0,true,30,simpleTimeout,1,false,true);

        //Calage Y
        actionList[ActionE::Start].addSPIN(STD,-PI/2.0,50,simpleTimeout,1,false);
        actionList[ActionE::Start].addGO_UNTIL(true,RECALLE,1.0,MessageE::Calle,200,simpleTimeout,1);
        actionList[ActionE::Start].addSETY(2.0-HROBOT,-PI/2.0,10,simpleTimeout,1);
        actionList[ActionE::Start].addSTBY(OFF,Impossible,3,normalTimeout,1);
        actionList[ActionE::Start].addGOTO(STD,0.1,0.1,0.0,0.0,true,30,simpleTimeout,1,false,true);

        //Placement
        startPoint1=init_vectorE(0.37,1.47,-0.23);
        startPoint2=init_vectorE(0.17,1.47,-0.23);
        if (coteViolet)
        {
            startPoint1=mirror(startPoint1);
            startPoint2=mirror(startPoint2);
        }
        actionList[ActionE::Start].addSPINGOTO(STD,startPoint1.vec.x,startPoint1.vec.y,100,simpleTimeout,1,false);
        actionList[ActionE::Start].addSPIN(STD,startPoint1.theta,100,simpleTimeout,1,false);
        actionList[ActionE::Start].addGOTO(STD,0.1,startPoint2,true,50,simpleTimeout,1,false,false);
        actionList[ActionE::Start].addSEND(MessageE::Pince_Extended,10,simpleTimeout,1);
        actionList[ActionE::Start].addSTBY(DYDM,Impossible,5,simpleTimeout,1);
        actionList[ActionE::Start].addSEND(MessageE::Start_Chaos,10,simpleTimeout,1);
        actionList[ActionE::Start].addSTBY(DYDM,MessageE::Tirette,60000,simpleTimeout,1);
        break;
    case Chaos:
        chaosPoint1=init_vectorE(0.8010540184453228,1.1963109354413701,-0.8998384686675115);
        chaosPoint2=init_vectorE(1.1251646903820818,0.7878787878787878,-0.7794810726914212);
        releasePoint=init_vectorE(0.3346508563899868,1.391304347826087,3.141592653589793);
        if (coteViolet)
        {
            chaosPoint1=mirror(chaosPoint1);
            chaosPoint2=mirror(chaosPoint2);
        }
        actionList[ActionE::Chaos] = Action(actionType);
        actionList[ActionE::Chaos].addSEND(MessageE::Pince_Half_Extended,10,simpleTimeout,1);
        actionList[ActionE::Chaos].addSTBY(DYDM,Impossible,5,normalTimeout,1);

        actionList[ActionE::Chaos].addGOTO(RUSH, 0.4,chaosPoint1, true,50,simpleTimeout,1,true,false);
        actionList[ActionE::Chaos].addSTBY(DYDM,Impossible,30,normalTimeout,1);
        actionList[ActionE::Chaos].addSEND(MessageE::Idle,10,simpleTimeout,1);
        actionList[ActionE::Chaos].addSTBY(DYDM,Impossible,2,normalTimeout,1);
        actionList[ActionE::Chaos].addGOTO(RUSH, 0.1,chaosPoint2, true,20,simpleTimeout,1,true,false);
        actionList[ActionE::Chaos].addSEND(MessageE::Pince_Half_Retracted,10,simpleTimeout,1);
        actionList[ActionE::Chaos].addSTBY(DYDM,Impossible,5,normalTimeout,1);

        actionList[ActionE::Chaos].addSPINGOTO(STD,releasePoint.vec.x,releasePoint.vec.y,100,simpleTimeout,1,false);
        actionList[ActionE::Chaos].addSEND(MessageE::Pince_Extended,10,simpleTimeout,1);
        actionList[ActionE::Chaos].addGOTO(STD,0.1,-0.4,0.0,0.0,true,50,simpleTimeout,1,true,true);


        actionList[ActionE::Chaos].addSTBY(DYDM,Impossible,50000,simpleTimeout,1);
        break;
    case Distribx6_1:
        actionList[actionType] = Action(actionType);

        actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
        break;
    case Distribx6_2:
        actionList[actionType] = Action(actionType);
        actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
        break;
    case Distribx6_3:
        actionList[actionType] = Action(actionType);
        actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
        break;
    case CoupDeCul:
        actionList[actionType] = Action(actionType);
        actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
        break;
    case Distribx3_1:
        actionList[actionType] = Action(actionType);
        actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
        break;
    case Distribx3_2:
        actionList[actionType] = Action(actionType);
        actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
        break;
    case RecupBlueAcc:
        actionList[actionType] = Action(actionType);
        actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
        break;
    case PoseAcc:
        actionList[actionType] = Action(actionType);
        actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
        break;
    case RecupGoldAcc:
        actionList[actionType] = Action(actionType);
        actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
        break;
    case Balance:
        actionList[actionType] = Action(actionType);
        actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
        break;
    case PoseSol:
        actionList[actionType] = Action(actionType);
        actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
        break;
    case CasseCouilles:
        actionList[actionType] = Action(actionType);
        actionList[actionType].addSEND(MessageE::New_Action,10,simpleTimeout,2);
        break;
    default:
        break;
    }
}

ActionE Cerveau::nextBestAction()
{
    if (currentActionIndex==ActionE::PoseSol)
    {
        for (int i=1;i<NB_ACTIONS;i++)
        {
            if (!(DONE[i]))
            {
                return(ActionE)i;
            }
        }
        return ActionE::PoseSol;
    }
    else
    {
        return (ActionE)((int)currentActionIndex+1);
    }

}

void Cerveau::abandonneCurrentAction()
{
#ifdef STATE
    Serial.println("ABANDON ACTION");
#endif

    DONE[currentActionIndex] = false;
    currentActionIndex = nextBestAction();
    ptrRobot->ordresFifo.clean();
    addActionOrders();
}

void Cerveau::actuate()
{
    if (actionList[currentActionIndex].actionCompleted)
    {
        DONE[currentActionIndex] = true;

        currentActionIndex = nextBestAction(); // Choisir une nouvelle action
        loadAction(currentActionIndex);
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
    }

    currentActionIndex = ActionE::Start;		// On commence avec le Chaos et on ajoute les ordres au buffer
    loadAction(ActionE::Start);
    addActionOrders();
}

Cerveau::~Cerveau()
{
}
