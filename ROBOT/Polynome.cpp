#include "Polynome.h"
#include "Arduino.h"

Polynome init_polynome()
{
  Polynome out;
  for (int i=0;i<=DEGRE_MAX;i++) {out.K[i]=0.0;}
  return out;
}

float Polynome::f(float x) 
{
  float out=0.0;
  float xn=1.0;
  for (int i=0;i<=DEGRE_MAX;i++) {out=out+K[i]*xn; xn=xn*x;}
  return out;
}

float Polynome::df(float x)
{
  float out=0.0;
  float xn=1.0;
  for (int i=1;i<=DEGRE_MAX;i++) {out=out+K[i]*xn*i; xn=xn*x;}
  return out;
}


void Polynome::set(float in_a0,float in_a1,float in_a2,float in_a3,float in_a4,float in_a5,float in_a6)
{
  K[0]=in_a0;
  K[1]=in_a1;
  K[2]=in_a2;
  K[3]=in_a3;
  K[4]=in_a4;
  K[5]=in_a5;
  K[6]=in_a6;
}


Polynome init_polynome(float a0,float a1,float a2,float a3,float a4,float a5,float a6)
{
  Polynome out;
  out.K[0]=a0;
  out.K[1]=a1;
  out.K[2]=a2;
  out.K[3]=a3;
  out.K[4]=a4;
  out.K[5]=a5;
  out.K[6]=a6;
  return out;
}

Polynome mult(Polynome P1,Polynome P2)
{
  Polynome out;
  out=init_polynome();
  for (int degreP=0;degreP<=DEGRE_MAX;degreP++)
  {
    for (int d=0;d<=degreP;d++)
    {
      out.K[degreP]=out.K[degreP]+P1.K[d]*P2.K[degreP-d];
    }
  }
  return out;
}

Polynome carre(Polynome P)
{
  Polynome out;
  out=init_polynome();
  out=mult(P,P);
  return out;
}

Polynome somme(Polynome P1,Polynome P2)
{
  Polynome out;
  out=init_polynome();
  for (int d=0;d<=DEGRE_MAX;d++) {out.K[d]=P1.K[d]+P2.K[d];}
  return out;
}

Polynome minus(Polynome P1,Polynome P2)
{
  Polynome out;
  out=init_polynome();
  for (int d=0;d<=DEGRE_MAX;d++) {out.K[d]=P1.K[d]-P2.K[d];}
  return out;
}

Polynome Pconstant(float y)
{
  Polynome out;
  out=init_polynome(y,0.0,0.0,0.0,0.0,0.0,0.0);
  return out;
}

Polynome derivee(Polynome P)
{
  Polynome out;
  out=init_polynome();
  for (int d=1;d<=DEGRE_MAX;d++) {out.K[d-1]=d*P.K[d];}
  return out;
}

Polynome multFAST(Polynome* P1,Polynome* P2)
{
  Polynome out;
  out=init_polynome();
  for (int degreP=0;degreP<=DEGRE_MAX;degreP++)
  {
    for (int d=0;d<=degreP;d++)
    {
      out.K[degreP]=out.K[degreP]+P1->K[d]*P2->K[degreP-d];
    }
  }
  return out;
}

Polynome carreFAST(Polynome* P)
{
  Polynome out;
  out=init_polynome();
  out=multFAST(P,P);
  return out;
}

Polynome sommeFAST(Polynome* P1,Polynome* P2)
{
  Polynome out;
  out=init_polynome();
  for (int d=0;d<=DEGRE_MAX;d++) {out.K[d]=P1->K[d]+P2->K[d];}
  return out;
}

Polynome minusFAST(Polynome* P1,Polynome* P2)
{
  Polynome out;
  out=init_polynome();
  for (int d=0;d<=DEGRE_MAX;d++) {out.K[d]=P1->K[d]-P2->K[d];}
  return out;
}

Polynome deriveeFAST(Polynome* P)
{
  Polynome out;
  out=init_polynome();
  for (int d=1;d<=DEGRE_MAX;d++) {out.K[d-1]=d*P->K[d];}
  return out;
}


void afficheP(Polynome P)
{
  for (int i=0;i<=DEGRE_MAX;i++)
  {
    Serial.print("X pow ");Serial.print(i);Serial.print(" ");Serial.println(P.K[i]);
  }
}

void S_function::set(float in_uIni,float in_uFin,float in_duIni,float in_duMax,float in_duFin,float in_dduIni,float in_dduFin)
{
  uIni=in_uIni;
  uFin=in_uFin;
  signe=(uFin-uIni<0)?(-1.0):(1.0);
  D=uFin-uIni;
  duIni=in_duIni;
  duFin=in_duFin;
  dduIni=signe*in_dduIni;
  dduFin=signe*in_dduFin;
  duMax=signe*min(in_duMax,sqrt( (2*D-duFin*duFin/dduFin+duIni*duIni/dduIni)/(1/dduIni-1/dduFin) ));
  t1=(duMax-duIni)/dduIni;
  if (D==0.0) t2=0.0;
  else t2=(D+t1*duMax-(duMax*duMax-duIni*duIni)/(2*dduIni)-(duFin*duFin-duMax*duMax)/(2*dduFin))/duMax;
  t3=(duFin-duMax)/dduFin+t2;
}

float S_function::f(float t)
{
  if (t<t1) return uIni+duIni*t+(duMax-duIni)*t*t/(2*t1);
  if (t<t2) return uIni+t1*(duIni+duMax)/2+duMax*(t-t1);
  if (t<t3) return uIni+t1*(duIni+duMax)/2+duMax*(t2-t1)+duMax*(t-t2)+(duFin-duMax)/(t3-t2)*( t*t/2-t2*t+t2*t2/2);
  return uFin;
}

float S_function::df(float t)
{
  if (t<t1) return duIni+(duMax-duIni)*t/t1;
  if (t<t2) return duMax;
  if (t<t3) return duMax+(duFin-duMax)/(t3-t2)*(t-t2);
  return duFin;
}

float S_function::ddf(float t)
{
  if (t<t1) return (duMax-duIni)/t1;
  if (t<t2) return 0.0;
  if (t<t3) return (duFin-duMax)/(t3-t2);
  return 0.0;
}
