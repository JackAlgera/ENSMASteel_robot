//Fifo pour FIRST IN FIRST OUT
//C'est ce fichier qui organise la succession des actions
#ifndef FIFO_INCLUDED
#define FIFO_INCLUDED

#include "1_CONSTANT.h"
#include "Vector.h"
#include "Arduino.h"
#include "Order.h"

class Fifo
{
  Order liste[TAILLEFIFO];
  uint8_t indiceDebut=1,indiceFin=0;
  
  public:
	  Fifo();
	  ~Fifo();
	  void add(Order order);
	  void addHead(Order order);
	  uint8_t inBuffer=0;
	  Order* ptrFst();          //Renvoie le pointeur vers l'action le plus ancienne (celle a effectuer normalement)
	  void pop();               //Fait sauter l'action la plus ancienne
	  void clean();				//Vide le buffer et ajoute un STBY

	  /* 
	  -----
	  Pas besoin de toussa
	  -----

	  void addGoto(uint8_t nerv,float fleche,float xAim,float yAim,float thetaAim,bool arret,uint8_t timeoutDs);  //abs(thetaAim-thetaIni)<=PI
	  void addSpin(uint8_t nerv,float thetaAim,uint8_t timeoutDs);
	  void addFWD(float acc,float v,uint8_t timeoutDs);
	  void addBWD(float acc,float v,uint8_t timeoutDs);
	  void addSTBY(uint8_t nerv,const char unlockMessage[],uint8_t timeoutDs);
	  void addEmStop(uint8_t timeout);
	  static Order createGoto(uint8_t nerv,float fleche,float xAim,float yAim,float thetaAim,bool arret,uint8_t timeoutDs);  //abs(thetaAim-thetaIni)<=PI
	  static Order createSpin(uint8_t nerv,float thetaAim,uint8_t timeoutDs);
	  static Order createFWD(float acc,float v,uint8_t timeoutDs);
	  static Order createBWD(float acc,float v,uint8_t timeoutDs);
	  static Order createSTBY(uint8_t nerv,const char unlockMessage[],uint8_t timeoutDs);
	  static Order createEmStop(uint8_t timeoutDs);
	  */
};

#endif
