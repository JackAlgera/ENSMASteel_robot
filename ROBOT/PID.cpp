#include "Vector.h"
#include "PID.h"
#include "Fifo.h"
#include "CONSTANT.h"


float nervA[3]=   { 0.1   , 0.8   , 1.5   };
float nervV[3]=   { 0.05  , 0.5   , 1.5   };
float nervTPP[3]= { 0.5   , 5.0   , 11.0  };
float nervTP[3]=  { 0.5   , 1.5   , 999.9  };

uint16_t K[5][2][3]=
  {//LINEAIRE               //ANGULAIRE  (PID)
  {{4100 , 0    , 1958} , {6700 , 50  , 450}},  //ACRT
  {{4100 , 0    , 1958} , {6700 , 50  , 450}},  //STD
  {{4100 , 0    , 1959} , {409 , 0  , 920}},    //RUSH
  {{5000 , 500  , 6000} , {5000 , 600  , 1200}}, //DYDM
  {{0,0,0} ,{0,0,0}} //OFF
  };

void PID::actuate(float dt,VectorE posERobot,float vRobot,float wRobot)
{
      
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
      float ordreA= (K[PIDnervANG][ANG][KP]*errorA + K[PIDnervANG][ANG][KI]*IA + K[PIDnervANG][ANG][KD]*(pointeurSurGhost->w - 0.8*0*wRobot))/1000.0;
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
      //On regarde si l'action est terminée
      if  (  
            (
            (
              (
                (longueur( minusFAST(&posERobot.vec,&pointeurSurGhost->posED.vec) )<=RAYON_TERMINE or (not PIDL)) 
                and ((normalize(pointeurSurGhost->posED.theta-posERobot.theta)<=DELTA_THETA_TERMINE) or (not PIDA))
              ) 
              or STATIQUE
            ) //geometrique
            and (abs(vRobot)<0.005 or STATIQUE or (pointeurSurFifo->ptrFst()->type==GOTO_TYPE and pointeurSurFifo->ptrFst()->goTo.arret==false)) //vitesse
            and pointeurSurGhost->t_e>0.95 //Le ghost a fini
            and (not pointeurSurGhost->locked)
            and  pointeurSurFifo->inBuffer>=2 //Il y a un suivant
            )
            or ((micros()-pointeurSurGhost->microsStart)/1000000.0  >   pointeurSurFifo->ptrFst()->timeoutDs/10.0 and  pointeurSurFifo->inBuffer>=2) //Il y a un suivant
          )
      {
        //On libere dans le cas d'un timeout
        pointeurSurGhost->locked=false;
        
        //On recalle le ghost sur le robot
        //if ( (not STATIQUE) and ((pointeurSurFifo->ptrFst()->type==GOTO_TYPE and pointeurSurFifo->ptrFst()->goTo.arret) or pointeurSurFifo->ptrFst()->type==SPIN_TYPE)   ) {IA=0.0;IL=0.0;pointeurSurGhost->recalle(posERobot,vRobot);}
        IA=0.0;IL=0.0;pointeurSurGhost->recalle(posERobot,vRobot);
        
        //On pop la liste
        pointeurSurFifo->pop();

        //On prepare le robot pour la prochaine action
        switch (pointeurSurFifo->ptrFst()->type)
        {
          case GOTO_TYPE:
          
          {
           GoTo g=pointeurSurFifo->ptrFst()->goTo;
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
          
          case SPIN_TYPE:
          {
          Spin s=pointeurSurFifo->ptrFst()->spin;
          PIDnervANG=s.nerv;
          PIDnervLIN=DYDM;
          pointeurSurGhost->X_P.set(pointeurSurGhost->posE.vec.x,0.0,0.0,0.0,0.0,0.0,0.0);
          pointeurSurGhost->Y_P.set(pointeurSurGhost->posE.vec.y,0.0,0.0,0.0,0.0,0.0,0.0);
          pointeurSurGhost->spinning=true; pointeurSurGhost->reversed=false;
          float thetaAimPropre=pointeurSurGhost->posE.theta+normalize(s.thetaAim-pointeurSurGhost->posE.theta);
          pointeurSurGhost->theta_S.set(pointeurSurGhost->posE.theta,thetaAimPropre,0.0,nervTP[s.nerv],0.0,nervTPP[s.nerv],-nervTPP[s.nerv]);
          pointeurSurGhost->t_e=0;}
          break;
          case FWD_TYPE:
          break;
          case BWD_TYPE:
          break;
          case STBY_TYPE:
          {
          PIDnervANG=pointeurSurFifo->ptrFst()->stby.nerv;
          PIDnervLIN=pointeurSurFifo->ptrFst()->stby.nerv;
          pointeurSurGhost->X_P.set(pointeurSurGhost->posE.vec.x,0.0,0.0,0.0,0.0,0.0,0.0);
          pointeurSurGhost->Y_P.set(pointeurSurGhost->posE.vec.y,0.0,0.0,0.0,0.0,0.0,0.0);
          pointeurSurGhost->spinning=true; pointeurSurGhost->reversed=false,pointeurSurGhost->locked=true;
          pointeurSurGhost->theta_S.set(pointeurSurGhost->posE.theta,pointeurSurGhost->posE.theta,0.0,1.0,0.0,1.0,1.0);
          pointeurSurGhost->t_e=0;}
          break;
        }
        pointeurSurGhost->microsStart=micros();
        pointeurSurGhost->t=0;
        pointeurSurGhost->actuate(dt);
        
      }
}

PID init_PID(Motor* in_pointeurSurMoteurGauche,Motor* in_pointeurSurMoteurDroite,Fifo* in_pointeurSurFifo,Ghost* in_pointeurSurGhost)
{
  PID out;
  out.pointeurSurMoteurGauche=in_pointeurSurMoteurGauche;
  out.pointeurSurMoteurDroite=in_pointeurSurMoteurDroite;
  out.pointeurSurFifo=in_pointeurSurFifo;
  out.pointeurSurGhost=in_pointeurSurGhost;
  return out;
}