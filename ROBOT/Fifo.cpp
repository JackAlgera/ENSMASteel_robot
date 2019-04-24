#include "Fifo.h"
#include "Vector.h"
#include "Comm.h"

void Fifo::add(Order order)
{
  indiceFin=(indiceFin+1)%TAILLEFIFO;
  inBuffer++;
  liste[indiceFin]=order;
}

void Fifo::addHead(Order order)
{
  indiceDebut=(indiceDebut-1+TAILLEFIFO)%TAILLEFIFO;
  inBuffer++;
  liste[indiceDebut]=order;
}

void Fifo::addGoto(uint8_t nerv,float fleche,float xAim,float yAim,float thetaAim,bool arret,uint8_t timeoutDs)
{
  Order ord;
  ord.type=GOTO_TYPE;
  ord.goTo.nerv=nerv;
  ord.goTo.fleche=fleche;         
  ord.goTo.posAim.x=xAim;
  ord.goTo.posAim.y=yAim;
  ord.goTo.thetaAim=thetaAim;
  ord.goTo.arret=arret;
  ord.timeoutDs=timeoutDs;
  add(ord);
}

void Fifo::addSpin(uint8_t nerv,float thetaAim,uint8_t timeoutDs)
{
  Order ord;
  ord.type=SPIN_TYPE;
  ord.spin.nerv=nerv;
  ord.spin.thetaAim=thetaAim;
  ord.timeoutDs=timeoutDs;
  add(ord);
}

void Fifo::addFWD(float acc,float v,uint8_t timeoutDs)
{
  Order ord;
  ord.type=FWD_TYPE;
  ord.fwd.acc=acc;
  ord.fwd.v=v;
  ord.timeoutDs=timeoutDs;
  add(ord);
}

void Fifo::addBWD(float acc,float v,uint8_t timeoutDs)
{
  Order ord;
  ord.type=BWD_TYPE;
  ord.bwd.acc=acc;
  ord.bwd.v=v;
  ord.timeoutDs=timeoutDs;
  add(ord);
}

void Fifo::addSTBY(uint8_t nerv,const char unlockMessage[],uint8_t timeoutDs)
{
  Order ord;
  ord.type=STBY_TYPE;
  ord.stby.nerv=nerv;
  strSet(ord.stby.unlockMessage,unlockMessage);
  ord.timeoutDs=timeoutDs;
  add(ord);
}

void Fifo::clean()
{
  indiceDebut=1;
  indiceFin=0;
  inBuffer=0;
  addSTBY(DYDM,"DUMY",255);
}

void Fifo::pop(){indiceDebut=(indiceDebut+1)%TAILLEFIFO;inBuffer--;}

Order* Fifo::ptrFst() {return &(liste[(indiceDebut)%TAILLEFIFO]);}

Fifo init_FIFO()
{
  Fifo out;
  return out;
}
