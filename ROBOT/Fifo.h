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
  public:
    Order liste[TAILLEFIFO];
    uint8_t indiceDebut = 1,indiceFin = 0, inBuffer = 0;
	  Fifo();
	  ~Fifo();
    
	  void add(Order order);
	  void addHead(Order order);
    void replaceHead(Order order);
	  Order* ptrFst();          //Renvoie le pointeur vers l'action le plus ancienne (celle a effectuer normalement)
	  void pop();               //Fait sauter l'action la plus ancienne
	  void clean();				//Vide le buffer et ajoute un STBY
    void printAll();

};

#endif
