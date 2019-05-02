#include "Vector.h"
#include "PID.h"
#include "Fifo.h"
#include "1_CONSTANT.h"
#include "Comm.h"
#include "Robot.h"
#include "Cerveau.h"
#include "ContreMesure.h"

float nervA[3]=   { 0.1, 0.8, 1.5   };
float nervV[3]=   { 0.05, 0.5, 1.5   };
float nervTPP[3]= { 0.5, 5.0, 11.0  };
float nervTP[3]=  { 0.5, 1.5, 999.9  };

uint16_t K[5][2][3]=
{
    //LINEAIRE               //ANGULAIRE  (PID)
    {{4100, 0, 1958}, {6700, 50, 450}},           //ACRT
    {{4100, 0, 1958}, {6700, 50, 450}},           //STD
    {{4100, 0, 1958}, {6700, 50, 450}},             //RUSH
    {{2500, 200, 9000}, {7000, 200, 1800}},        //DYDM
    {{0,0,0},{0,0,0}}  //OFF
};

void PID::failureDetected(ErreurE erreur)
{
  ptrRobot->ordresFifo.ptrFst()->nbFail++;
  if(ptrRobot->ordresFifo.ptrFst()->nbFail>ptrRobot->ordresFifo.ptrFst()->nbMaxFail)
  {
    if(ptrRobot->ordresFifo.ptrFst()->ptrActionPere!=nullptr){ptrRobot->master->abandonneCurrentAction();}
    else{loadNext();}
  }
  else
  {
    bool sanction=(*(ptrRobot->ordresFifo.ptrFst()->contreMesure))(ptrRobot,erreur);
    if (!sanction)ptrRobot->ordresFifo.ptrFst()->nbFail--;
  }
}

void PID::reload()
{
    float dt=lastDt;
    VectorE posERobot=lastPosERobot;
    float vRobot=lastVRobot;
    //On libere dans le cas d'un timeout
    ptrRobot->ghost.locked=false;
    //On suppose dans un premier temps qu'on est nearEnough
    timeLastNearEnough=millis()/1000.0;

    //On debypass
    ptrRobot->moteurGauche.bypass=false;
    ptrRobot->moteurDroite.bypass=false;

    //On recalle le ghost sur le robot
    IA=0.0;
    IL=0.0;
    if (not STATIQUE)
        ptrRobot->ghost.recalle(posERobot,vRobot);

    //On prepare le robot pour la prochaine action
    switch (ptrRobot->ordresFifo.ptrFst()->type)
    {
    case OrderE::GOTO_E:

    {
#ifdef STATE
        Serial.println("Je prepare un GOTO");
#endif
        GOTO_S g=ptrRobot->ordresFifo.ptrFst()->goTo;
        PIDnervLIN=g.nerv;
        PIDnervANG=(g.nerv==RUSH)?(DYDM):(g.nerv);
        //Ici on trouve les points a renseigner pour faire la trajectoire
        float L=longueur(minus(g.posAim,ptrRobot->ghost.posE.vec));
        {
            //BLOC GEOMETRIE BEZIER
            float x0=ptrRobot->ghost.posE.vec.x;
            float y0=ptrRobot->ghost.posE.vec.y;
            float thetaIni=ptrRobot->ghost.posE.theta;
            float x3=g.posAim.x;
            float y3=g.posAim.y;
            float thetaAim=g.thetaAim;
            float delta;
            if (sin(thetaAim-thetaIni)!=0.0)
            {
                Vector I=intersection(x0,y0,thetaIni,x3,y3,thetaAim);
                delta=max(  longueur(minusFAST(&g.posAim,&I)), longueur(minusFAST(&ptrRobot->ghost.posE.vec,&I))   );
            }
            else
                delta=9.9;//infini sur une table de 2x3

            float x1=x0+cos(thetaIni)*min(g.fleche*L,delta);
            float y1=y0+sin(thetaIni)*min(g.fleche*L,delta);
            float x2=x3-cos(thetaAim)*min(g.fleche*L,delta);
            float y2=y3-sin(thetaAim)*min(g.fleche*L,delta);

            //On actualise les polynomes
            ptrRobot->ghost.X_P.set(x0, -3*(x0-x1), 3*(x0-2*x1+x2), -1*(x0-3*x1+3*x2-x3),0.0,0.0,0.0);
            ptrRobot->ghost.Y_P.set(y0, -3*(y0-y1), 3*(y0-2*y1+y2), -1*(y0-3*y1+3*y2-y3),0.0,0.0,0.0);
        }  //BLOC GEOMETRIE BEZIER
        {
            //BLOC POLYNOMES
            Polynome DX=deriveeFAST(&ptrRobot->ghost.X_P);
            Polynome DDX=deriveeFAST(&DX);
            Polynome DY=deriveeFAST(&ptrRobot->ghost.Y_P);
            Polynome DDY=deriveeFAST(&DY);
            ptrRobot->ghost.v_e_P2=somme(  carreFAST(&DX), carreFAST(&DY) );
            ptrRobot->ghost.DDX_P=DDX;
            ptrRobot->ghost.DDY_P=DDY;

        }  //BLOC POLYNOMES
        ptrRobot->ghost.spinning=false;
        ptrRobot->ghost.reversed=false;
        float D=0.0,t_e_integral=0.0,v=0.0;
        float lastV=sqrt(ptrRobot->ghost.v_e_P2.f(t_e_integral));
        float pas=0.01;
        t_e_integral=t_e_integral+pas;
        while (t_e_integral<=1.0)
        {
            v=sqrt(ptrRobot->ghost.v_e_P2.f(t_e_integral));
            D=D+(v+lastV)/2*pas;
            lastV=v;
            t_e_integral=t_e_integral+pas;
        }
        ptrRobot->ghost.s.set(0.0,D,ptrRobot->ghost.v,nervV[g.nerv], (g.arret)?(0.0):(nervV[g.nerv]),nervA[g.nerv],-1.5*nervA[g.nerv]);
        if (L>0)
            ptrRobot->ghost.t_e=0;
        else
            ptrRobot->ghost.t_e=1;
    }
    break;

    case OrderE::SPIN_E:
    {
#ifdef STATE
        Serial.println("Je prepare un SPIN");
#endif
        SPIN_S s=ptrRobot->ordresFifo.ptrFst()->spin;
        PIDnervANG=s.nerv;
        PIDnervLIN=DYDM;
        ptrRobot->ghost.X_P.set(ptrRobot->ghost.posE.vec.x,0.0,0.0,0.0,0.0,0.0,0.0);
        ptrRobot->ghost.Y_P.set(ptrRobot->ghost.posE.vec.y,0.0,0.0,0.0,0.0,0.0,0.0);
        ptrRobot->ghost.spinning=true;
        ptrRobot->ghost.reversed=false;
        float thetaAimPropre=ptrRobot->ghost.posE.theta+normalize(s.thetaAim-ptrRobot->ghost.posE.theta);
        ptrRobot->ghost.theta_S.set(ptrRobot->ghost.posE.theta,thetaAimPropre,0.0,nervTP[s.nerv],0.0,nervTPP[s.nerv],-nervTPP[s.nerv]);
        ptrRobot->ghost.t_e=0;
    }
    break;
    case OrderE::SPINGOTO_E:
    {
#ifdef STATE
        Serial.println("Je prepare un SPINGOTO");
#endif
        SPINGOTO_S sg=ptrRobot->ordresFifo.ptrFst()->spinGoTo;
        Vector delta=minus(sg.posAim,posERobot.vec);
        Vector aim=sg.posAim;
        float nerv=sg.nerv;
        Action * papa=ptrRobot->ordresFifo.ptrFst()->ptrActionPere;
        ptrFonction contreMesure=ptrRobot->ordresFifo.ptrFst()->contreMesure;
        float timeout=ptrRobot->ordresFifo.ptrFst()->timeoutDs;
        Serial.print("angle ");
        Serial.println(angle(delta));
        ptrRobot->ordresFifo.replaceHead(GOTO(nerv,0.1,aim.x, aim.y, angle(delta), true, timeout,papa,contreMesure,0));
        ptrRobot->ordresFifo.addHead(SPIN(nerv,angle(delta),timeout,nullptr,contreMesure,0));
        reload();
    }
    break;
    case OrderE::FWD_E:
        break;
    case OrderE::BWD_E:
        break;
    case OrderE::STBY_E:
    {
#ifdef STATE
        Serial.println("Je prepare un STBY");
#endif
        PIDnervANG=ptrRobot->ordresFifo.ptrFst()->stby.nerv;
        PIDnervLIN=ptrRobot->ordresFifo.ptrFst()->stby.nerv;
        ptrRobot->ghost.X_P.set(ptrRobot->ghost.posE.vec.x,0.0,0.0,0.0,0.0,0.0,0.0);
        ptrRobot->ghost.Y_P.set(ptrRobot->ghost.posE.vec.y,0.0,0.0,0.0,0.0,0.0,0.0);
        ptrRobot->ghost.spinning=true;
        ptrRobot->ghost.reversed=false,ptrRobot->ghost.locked=true;
        ptrRobot->ghost.theta_S.set(ptrRobot->ghost.posE.theta,ptrRobot->ghost.posE.theta,0.0,1.0,0.0,1.0,1.0);
        ptrRobot->ghost.t_e=0;
    }
    break;
    case OrderE::SEND_E:
#ifdef STATE
        Serial.println("Je prepare un SEND");
#endif
        ptrRobot->comm.send(ptrRobot->ordresFifo.ptrFst()->send.message);
        loadNext();
        break;
    case OrderE::EMSTOP_E:
#ifdef STATE
        Serial.println("Je prepare un EMSTOP");
#endif
        ptrRobot->moteurGauche.bypass=true;
        ptrRobot->moteurGauche.masterOrder=0;
        ptrRobot->moteurDroite.bypass=true;
        ptrRobot->moteurDroite.masterOrder=0;
        break;
    }
    ptrRobot->ghost.microsStart=micros();
    ptrRobot->ghost.t=0;
    ptrRobot->ghost.actuate(dt);
}

void PID::actuate(float dt,VectorE posERobot,float vRobot,float wRobot)
{
    lastDt=dt;
    lastPosERobot=posERobot;
    lastVRobot=vRobot;
    {
        //Calcul de l'erreur linéaire avec retard
        float errorL=cross( minus(ptrRobot->ghost.posED.vec,posERobot.vec), directeur(posERobot.theta));
        if (errorL<0)
        {
            errorL=-1*sqrt(-1*errorL);
        }
        else
        {
            errorL=sqrt(errorL);
        }

        //Calcul de l'erreur angulaire avec retard
        float errorA;
        // Si le robot est proche du ghost, le robot imite la direction du ghost
        if (abs(errorL)<RAYON_RECONVERGENCE or STATIQUE)
        {
            reconvergence=false;
            errorA=normalize(ptrRobot->ghost.posED.theta-posERobot.theta);
        }
        // Sinon, le robot converge vers le ghost
        else
        {
            reconvergence=true;
            if (errorL>=0)
                errorA=normalize( angle(minus(ptrRobot->ghost.posED.vec,posERobot.vec)) -  posERobot.theta);
            else
                errorA=normalize( angle(minus(ptrRobot->ghost.posED.vec,posERobot.vec)) -  posERobot.theta+PI);
        }

        IL+=errorL*dt;
        IA+=errorA*dt;

        //PD
        float ordreA= (K[PIDnervANG][ANG][KP]*errorA + K[PIDnervANG][ANG][KI]*IA + K[PIDnervANG][ANG][KD]*(ptrRobot->ghost.w - 0.8*wRobot))/1000.0;
        ordreA=constrain(ordreA*MAXPWM,-MAXPWM, MAXPWM);

        float ordreL= (K[PIDnervLIN][LIN][KP]*errorL + K[PIDnervLIN][LIN][KI]*IL   + K[PIDnervLIN][LIN][KD]*(ptrRobot->ghost.v - 0.95*vRobot))/1000.0;
        ordreL=constrain(ordreL*MAXPWM,-MAXPWM+abs(ordreA),MAXPWM-abs(ordreA));

        //On donne les ordres
        ptrRobot->moteurGauche.order= (int32_t)(ordreL - ordreA);
        ptrRobot->moteurDroite.order= (int32_t)(ordreL + ordreA);

        //On actualise
        ptrRobot->moteurGauche.actuate();
        ptrRobot->moteurDroite.actuate();

#ifdef PIDSETUP
        Serial.print(ordreA); //BLEU
        Serial.print(";");
        Serial.print(ordreL); //ORANGE
        Serial.print(";");
        Serial.print(1); //VERT
        Serial.print(";");
        Serial.print(1); //ROUGE
        Serial.print(";");
        Serial.print(1); //VIOLET
        Serial.print("!");
#endif

    }

    bool nearEnough   = (longueur( minusFAST(&posERobot.vec,&ptrRobot->ghost.posED.vec) )<=RAYON_FAIL and (normalize(ptrRobot->ghost.posED.theta-posERobot.theta)<=DELTA_THETA_FAIL or reconvergence)) or STATIQUE;
    bool linOK        = longueur( minusFAST(&posERobot.vec,&ptrRobot->ghost.posED.vec) )<=RAYON_TERMINE or STATIQUE;
    bool angOK        = (normalize(ptrRobot->ghost.posED.theta-posERobot.theta)<=DELTA_THETA_TERMINE) or STATIQUE;
    bool vitesseOK    = (abs(vRobot)<0.005 and abs(wRobot)<0.005) or STATIQUE or (ptrRobot->ordresFifo.ptrFst()->type == OrderE::GOTO_E and ptrRobot->ordresFifo.ptrFst()->goTo.arret==false);
    bool ghostArrive  = ptrRobot->ghost.t_e>0.95;
    bool ghostFree    = not ptrRobot->ghost.locked;
    //bool orderNext    = ptrRobot->ordresFifo.inBuffer>=2;
    
    bool timeout      = (micros()-ptrRobot->ghost.microsStart)/1000000.0  >   ptrRobot->ordresFifo.ptrFst()->timeoutDs/10.0;
    bool messageITSTBY  = ptrRobot->ordresFifo.ptrFst()->type == OrderE::STBY_E and strEqual(ptrRobot->comm.lastMessage,ptrRobot->ordresFifo.ptrFst()->stby.unlockMessage);
    bool completeEMStop = ptrRobot->ordresFifo.ptrFst()->type == OrderE::EMSTOP_E and abs(vRobot)<0.005 and abs(wRobot)<0.005;
    
    if(timeout){failureDetected(ErreurE::TIMEOUT);} 
    if(nearEnough){timeLastNearEnough=millis()/1000.0;}
    if(millis()/1000.0-timeLastNearEnough>FAIL_TIME)
    {
      failureDetected(ErreurE::PID_FAILURE);
      timeLastNearEnough=millis()/1000.0;   //Petit repis
    }

    

    //On regarde si l'action est terminée
    if  (
        (linOK and angOK and vitesseOK and ghostArrive and ghostFree)// and orderNext)   //cas standard
        or (messageITSTBY) //and orderNext)                                              //cas message
        or (completeEMStop) //and orderNext)                                             //cas EMSTOP
        )
    {
        //On vide la boite au lettre si on est sorti du stby grace a un message
        if (messageITSTBY)
            ptrRobot->comm.taken();

        loadNext();
    }
}

void PID::loadNext()
{
    //On fait avancer l'action si necessaire
    if (ptrRobot->ordresFifo.ptrFst()->ptrActionPere!=nullptr)
        ptrRobot->ordresFifo.ptrFst()->ptrActionPere->nextStep();

    //On pop le Fifo
    ptrRobot->ordresFifo.pop();
    if (ptrRobot->ordresFifo.inBuffer==0){ptrRobot->ordresFifo.add(STBY(DYDM, "DUMY", 1, nullptr,normalTimeout,0));} //TODO verifier si OFF n'est pas mieux

    //On dit au robot que l'ordre actuel a change
    reload();
}

PID::PID()
{
}

PID::PID(Robot * ptrRobot)
{
    this->ptrRobot=ptrRobot;
    this->timeLastNearEnough=millis()/1000.0;
}

PID::~PID()
{
}
