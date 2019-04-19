#include "Ghost.h"
#include "Arduino.h"

void Ghost::recalle(VectorE posERobot, float vRobot)
{
  posE=posERobot;
  v=vRobot;
}

void Ghost::actuate(float dt)
{
  #ifndef RC
  t=t+dt;
  if (dt>0.0001)
  {
  if (spinning==false)
  {
    v_e=sqrt(v_e_P2.f(t_e));
    v=s.df(t);
    if (v_e!=0.0) t_e+=s.df(t)/v_e*dt;
    if (t_e>1.0) t_e=1.0;
    posE.vec.x=X_P.f(t_e);
    posE.vec.y=Y_P.f(t_e);
    posE.theta=atan2(Y_P.df(t_e),X_P.df(t_e));
    if (v_e>0.0001) {wF.in((DDY_P.f(t_e)*cos(posE.theta) - DDX_P.f(t_e)*sin(posE.theta)) *   v/(v_e*v_e),dt);w=wF.out();} else w=0; 
  }
  else
  {
    posE.theta=normalize(theta_S.f(t));
    w=theta_S.df(t);
    if (theta_S.t3==0.0) t_e=1;
    else t_e=t/theta_S.t3;
    if (t_e>1.0) t_e=1.0;
    v=0.0;
  }
  #endif //RC
  fDelayX.in(posE.vec.x,dt);posED.vec.x=fDelayX.out();
  fDelayY.in(posE.vec.y,dt);posED.vec.y=fDelayY.out();
  fDelayTheta.in(posE.theta,dt);posED.theta=fDelayTheta.out();
  }
}

void Ghost::unlock()
{
  locked=false;
}

Ghost init_ghost(VectorE posEini)
{
  Ghost out;
  out.fDelayX=newFiltre(posEini.vec.x,DELAY,3);
  out.fDelayY=newFiltre(posEini.vec.y,DELAY,3);
  out.fDelayTheta=newFiltre(posEini.theta,DELAY,3); //L'ordre 3 est un simple retard
  out.wF=newFiltre(0.0,1.0,2);
  out.t=0;
  out.microsStart=micros();
  out.posE=posEini;
  out.posED=posEini;
  out.v=0.0;
  out.w=0.0;
  out.X_P=Pconstant(posEini.vec.x);
  out.Y_P=Pconstant(posEini.vec.y);
  out.v_e_P2=Pconstant(0.0);
  out.s.set(0.0,0.0,0.0,1.0,0.0,1.0,-1.0);
  out.theta_S.set(posEini.theta,posEini.theta,0.0,1.0,0.0,1.0,-1.0);
  return out;
}
