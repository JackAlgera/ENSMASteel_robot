#include "Vector.h"
#include "PID.h"
#include "Fifo.h"
#include "1_CONSTANT.h"
#include "Comm.h"

float nervA[3]=   { 0.1   , 0.8   , 1.5   };
float nervV[3]=   { 0.05  , 0.5   , 1.5   };
float nervTPP[3]= { 0.5   , 5.0   , 11.0  };
float nervTP[3]=  { 0.5   , 1.5   , 999.9  };

uint16_t K[5][2][3]=
  {//LINEAIRE               //ANGULAIRE  (PID)
  {{4100 , 0    , 1958} , {6700 , 50  , 450}},  //ACRT
  {{4100 , 0    , 1958} , {6700 , 50  , 450}},  //STD
  {{4100 , 0    , 1958} , {6700 , 50  , 450}},    //RUSH
  {{2500 , 200  , 9000} , {7000 , 200  , 1800}}, //DYDM
  {{0,0,0} ,{0,0,0}} //OFF
  };

void PID::reload()
{       
        float dt=lastDt;
        VectorE posERobot=lastPosERobot;
        float vRobot=lastVRobot;
        //On libere dans le cas d'un timeout
        pointeurSurGhost->locked=false;

        //On debypass
        pointeurSurMoteurGauche->bypass=false;
        pointeurSurMoteurDroite->bypass=false;
        
        //On recalle le ghost sur le robot
        //if ( (not STATIQUE) and ((pointeurSurFifo->ptrFst()->type==GOTO_TYPE and pointeurSurFifo->ptrFst()->goTo.arret) or pointeurSurFifo->ptrFst()->type==SPIN_TYPE)   ) {IA=0.0;IL=0.0;pointeurSurGhost->recalle(posERobot,vRobot);}
        IA=0.0;IL=0.0;if (not STATIQUE) pointeurSurGhost->recalle(posERobot,vRobot);

        //On prepare le robot pour la prochaine action
        switch (pointeurSurFifo->ptrFst()->type)
        {
		case OrderE::GOTO_E:
          
          {
          #ifdef STATE
          Serial.println("Je prepare un GOTO");
          #endif
          GOTO_S g=pointeurSurFifo->ptrFst()->goTo;
          PIDnervLIN=g.nerv;
          PIDnervANG=(g.nerv==RUSH)?(DYDM):(g.nerv);
          //Ici on trouve les points a renseigner pour faire la trajectoire
          float L=longueur(minus(g.posAim,pointeurSurGhost->posE.vec));
        { //BLOC GEOMETRIE BEZIER
          float x0=pointeurSurGhost->posE.vec.x;float y0=pointeurSurGhost->posE.vec.y;float thetaIni=pointeurSurGhost->posE.theta;
          float x3=g.posAim.x;float y3=g.posAim.y;float thetaAim=g.thetaAim;
          float delta;
          if (sin(thetaAim-thetaIni)!=0.0){
            Vector I=intersection(x0,y0,thetaIni,x3,y3,thetaAim);
            delta=max(  longueur(minusFAST(&g.posAim,&I))  , longueur(minusFAST(&pointeurSurGhost->posE.vec,&I))   );}
          else 
            delta=9.9;//infini sur une table de 2x3
          
          float x1=x0+cos(thetaIni)*min(g.fleche*L,delta);float y1=y0+sin(thetaIni)*min(g.fleche*L,delta);
          float x2=x3-cos(thetaAim)*min(g.fleche*L,delta);float y2=y3-sin(thetaAim)*min(g.fleche*L,delta);

          //On actualise les polynomes
          pointeurSurGhost->X_P.set(x0 , -3*(x0-x1) , 3*(x0-2*x1+x2) , -1*(x0-3*x1+3*x2-x3),0.0,0.0,0.0);
          pointeurSurGhost->Y_P.set(y0 , -3*(y0-y1) , 3*(y0-2*y1+y2) , -1*(y0-3*y1+3*y2-y3),0.0,0.0,0.0);
       }  //BLOC GEOMETRIE BEZIER
       {  //BLOC POLYNOMES
          Polynome DX=deriveeFAST(&pointeurSurGhost->X_P);
          Polynome DDX=deriveeFAST(&DX);
          Polynome DY=deriveeFAST(&pointeurSurGhost->Y_P);
          Polynome DDY=deriveeFAST(&DY);
          pointeurSurGhost->v_e_P2=somme(  carreFAST(&DX)  , carreFAST(&DY) );
          pointeurSurGhost->DDX_P=DDX;
          pointeurSurGhost->DDY_P=DDY;

       }  //BLOC POLYNOMES
          pointeurSurGhost->spinning=false; pointeurSurGhost->reversed=false;
          float D=0.0,t_e_integral=0.0,v=0.0;
          float lastV=sqrt(pointeurSurGhost->v_e_P2.f(t_e_integral));
          float pas=0.01;
          t_e_integral=t_e_integral+pas;
          while (t_e_integral<=1.0)
          {
            v=sqrt(pointeurSurGhost->v_e_P2.f(t_e_integral));
            D=D+(v+lastV)/2*pas;
            lastV=v;
            t_e_integral=t_e_integral+pas;
          }
          pointeurSurGhost->s.set(0.0,D,pointeurSurGhost->v,nervV[g.nerv], (g.arret)?(0.0):(nervV[g.nerv]) ,nervA[g.nerv],-1.5*nervA[g.nerv]);
          if (L>0) pointeurSurGhost->t_e=0; else pointeurSurGhost->t_e=1;}
          break;
          
		case OrderE::SPIN_E:
          {
          #ifdef STATE
          Serial.println("Je prepare un SPIN");
          #endif
          SPIN_S s=pointeurSurFifo->ptrFst()->spin;
          PIDnervANG=s.nerv;
          PIDnervLIN=DYDM;
          pointeurSurGhost->X_P.set(pointeurSurGhost->posE.vec.x,0.0,0.0,0.0,0.0,0.0,0.0);
          pointeurSurGhost->Y_P.set(pointeurSurGhost->posE.vec.y,0.0,0.0,0.0,0.0,0.0,0.0);
          pointeurSurGhost->spinning=true; pointeurSurGhost->reversed=false;
          float thetaAimPropre=pointeurSurGhost->posE.theta+normalize(s.thetaAim-pointeurSurGhost->posE.theta);
          pointeurSurGhost->theta_S.set(pointeurSurGhost->posE.theta,thetaAimPropre,0.0,nervTP[s.nerv],0.0,nervTPP[s.nerv],-nervTPP[s.nerv]);
          pointeurSurGhost->t_e=0;}
          break;
    case OrderE::SPINGOTO_E:
          {
            #ifdef STATE
            Serial.println("Je prepare un SPINGOTO");
            #endif
            SPINGOTO_S sg=pointeurSurFifo->ptrFst()->spinGoTo;
            Vector delta=minus(sg.posAim,posERobot.vec);
            Vector aim=sg.posAim;
            float nerv=sg.nerv;
            float timeout=pointeurSurFifo->ptrFst()->timeoutDs;
            Serial.print("angle ");Serial.println(angle(delta));
            pointeurSurFifo->replaceHead(GOTO(nerv,0.1,aim.x, aim.y, angle(delta), true, timeout));
            pointeurSurFifo->addHead(SPIN(nerv,angle(delta),timeout));
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
          PIDnervANG=pointeurSurFifo->ptrFst()->stby.nerv;
          PIDnervLIN=pointeurSurFifo->ptrFst()->stby.nerv;
          pointeurSurGhost->X_P.set(pointeurSurGhost->posE.vec.x,0.0,0.0,0.0,0.0,0.0,0.0);
          pointeurSurGhost->Y_P.set(pointeurSurGhost->posE.vec.y,0.0,0.0,0.0,0.0,0.0,0.0);
          pointeurSurGhost->spinning=true; pointeurSurGhost->reversed=false,pointeurSurGhost->locked=true;
          pointeurSurGhost->theta_S.set(pointeurSurGhost->posE.theta,pointeurSurGhost->posE.theta,0.0,1.0,0.0,1.0,1.0);
          pointeurSurGhost->t_e=0;}
          break;
		case OrderE::POST_E:
		  break;
		case OrderE::EMSTOP_E:
          #ifdef STATE
          Serial.println("Je prepare un EMSTOP");
          #endif
          pointeurSurMoteurGauche->bypass=true;
          pointeurSurMoteurGauche->masterOrder=0;
          pointeurSurMoteurDroite->bypass=true;
          pointeurSurMoteurDroite->masterOrder=0;
          break;
        }
        pointeurSurGhost->microsStart=micros();
        pointeurSurGhost->t=0;
        pointeurSurGhost->actuate(dt);
}

void PID::actuate(float dt,VectorE posERobot,float vRobot,float wRobot)
{
      lastDt=dt;lastPosERobot=posERobot;lastVRobot=vRobot;
{
      //Calcul de l'erreur linéaire avec retard
      float errorL=cross( minus(pointeurSurGhost->posED.vec,posERobot.vec) , directeur(posERobot.theta));
      if (errorL<0) {errorL=-1*sqrt(-1*errorL);} else {errorL=sqrt(errorL);}
      
      //Calcul de l'erreur angulaire avec retard
      float errorA;
      // Si le robot est proche du ghost, le robot imite la direction du ghost
      if (abs(errorL)<RAYON_RECONVERGENCE or STATIQUE or (not PIDL)) errorA=normalize(pointeurSurGhost->posED.theta-posERobot.theta);
      // Sinon, le robot converge vers le ghost
      else 
      {
        if (errorL>=0) 
          errorA=normalize( angle(minus(pointeurSurGhost->posED.vec,posERobot.vec)) -  posERobot.theta); 
        else
          errorA=normalize( angle(minus(pointeurSurGhost->posED.vec,posERobot.vec)) -  posERobot.theta+PI); 
      }

      IL+=errorL*dt;IA+=errorA*dt;

      //PD
      float ordreA= (K[PIDnervANG][ANG][KP]*errorA + K[PIDnervANG][ANG][KI]*IA + K[PIDnervANG][ANG][KD]*(pointeurSurGhost->w - 0.8*wRobot))/1000.0;
      if (not PIDA) ordreA=0.0; else ordreA=constrain(ordreA*MAXPWM,-MAXPWM , MAXPWM);
      
      float ordreL= (K[PIDnervLIN][LIN][KP]*errorL + K[PIDnervLIN][LIN][KI]*IL   + K[PIDnervLIN][LIN][KD]*(pointeurSurGhost->v - 0.95*vRobot))/1000.0;
      if (not PIDL) ordreL=0.0; else ordreL=constrain(ordreL*MAXPWM,-MAXPWM+abs(ordreA),MAXPWM-abs(ordreA));
      
      //On donne les ordres
      pointeurSurMoteurGauche->order= (int)(ordreL - ordreA);
      pointeurSurMoteurDroite->order= (int)(ordreL + ordreA);

      //On actualise
      pointeurSurMoteurGauche->actuate();
      pointeurSurMoteurDroite->actuate();
      
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
      
      bool linOK        = longueur( minusFAST(&posERobot.vec,&pointeurSurGhost->posED.vec) )<=RAYON_TERMINE or (not PIDL) or STATIQUE;
      bool angOK        = (normalize(pointeurSurGhost->posED.theta-posERobot.theta)<=DELTA_THETA_TERMINE) or (not PIDA) or STATIQUE;
      bool vitesseOK    = (abs(vRobot)<0.005 and abs(wRobot)<0.005) or STATIQUE or (pointeurSurFifo->ptrFst()->type == OrderE::GOTO_E and pointeurSurFifo->ptrFst()->goTo.arret==false);
      bool ghostArrive  = pointeurSurGhost->t_e>0.95;
      bool ghostFree    = not pointeurSurGhost->locked;
      bool orderNext    = pointeurSurFifo->inBuffer>=2;
      bool timeout      = (micros()-pointeurSurGhost->microsStart)/1000000.0  >   pointeurSurFifo->ptrFst()->timeoutDs/10.0;
      bool messageITSTBY  = pointeurSurFifo->ptrFst()->type == OrderE::STBY_E and strEqual(pointeurSurComm->lastMessage,pointeurSurFifo->ptrFst()->stby.unlockMessage);
      bool completeEMStop = pointeurSurFifo->ptrFst()->type == OrderE::EMSTOP_E and abs(vRobot)<0.005 and abs(wRobot)<0.005;


      //On regarde si l'action est terminée
      if  ( 
              (linOK and angOK and vitesseOK and ghostArrive and ghostFree and orderNext)   //cas standard
              or (timeout and orderNext)                                                    //cas timeout
              or (messageITSTBY and orderNext)                                              //cas message
              or (completeEMStop and orderNext)                                             //cas EMSTOP
          )
      {
        //On pop la liste
        pointeurSurFifo->pop();

        //On vide la boite au lettre si on est sorti du stby grace a un message
        if (messageITSTBY)pointeurSurComm->taken();

        //On met le robot a jour
        reload();
      }
}

PID init_PID(Motor* in_pointeurSurMoteurGauche,Motor* in_pointeurSurMoteurDroite,Fifo* in_pointeurSurFifo,Ghost* in_pointeurSurGhost,Comm* in_pointeurSurComm)
{
  PID out;
  out.pointeurSurMoteurGauche=in_pointeurSurMoteurGauche;
  out.pointeurSurMoteurDroite=in_pointeurSurMoteurDroite;
  out.pointeurSurFifo=in_pointeurSurFifo;
  out.pointeurSurGhost=in_pointeurSurGhost;
  out.pointeurSurComm=in_pointeurSurComm;
  return out;
}
