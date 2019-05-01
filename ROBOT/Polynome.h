#ifndef POLYNOME_INCLUDED
#define POLYNOME_INCLUDED

#include "1_CONSTANT.h"

class Polynome
{
  public:
  float K[DEGRE_MAX+1];        //Les 10 coefficients du polynome, K[i] est le coefficient devant x^i 
  float f(float x);   //Donne la valeur du polynome en x
  float df(float x);  //Donne la valeur de la dérivée du polynome en x
  float ddf(float x);
  void set(float a0,float a1,float a2,float a3,float a4,float a5,float a6);
};



class S_function
{
  float signe=1.0;
  public:
  float uIni,uFin,duIni,duMax,duFin,dduIni,dduFin,t1,t2,t3,D;
  // uInitial,uFinal,deriveDeUMax,deriveDeUInitial,deriveDeUFinal,deriveSecondeDeUInitial,deriveSecondeDeUFinale,temps fin accel, temps debut decel,temps fin decel,distance du parcours
  float f(float t);   //Valeur de la fonction
  float df(float t);  //Valeur de la derivee de la fonction
  float ddf(float t); //Valeur de la derivee seconde de la fonction
  void set(float in_uIni,float in_uFin,float in_duIni,float in_duMax,float in_duFin,float in_dduIni,float in_dduFin);
};

Polynome init_polynome();
void afficheP(Polynome P);
Polynome derivee(Polynome P);
Polynome init_polynome(float a0,float a1,float a2,float a3,float a4,float a5,float a6);  //Crée un nouveau polynome de degre 10 max
Polynome Pconstant(float y);              //Polynome constant de valeur y
Polynome mult(Polynome P1,Polynome P2);   //Multiplication de polynomes
Polynome carre(Polynome P);               //Met le polynome au carré
Polynome somme(Polynome P1,Polynome P2);  //Somme deux polynomes
Polynome minus(Polynome P1,Polynome P2);

Polynome deriveeFAST(Polynome* P);
Polynome multFAST(Polynome* P1,Polynome* P2);   //Multiplication de polynomes
Polynome carreFAST(Polynome* P);               //Met le polynome au carré
Polynome sommeFAST(Polynome* P1,Polynome* P2);  //Somme deux polynomes
Polynome minusFAST(Polynome* P1,Polynome* P2);
#endif
