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

Order Fifo::createGoto(uint8_t nerv,float fleche,float xAim,float yAim,float thetaAim,bool arret,uint8_t timeoutDs)
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
  return(ord);
}

void Fifo::addGoto(uint8_t nerv,float fleche,float xAim,float yAim,float thetaAim,bool arret,uint8_t timeoutDs)
{add(createGoto(nerv,fleche,xAim,yAim,thetaAim,arret,timeoutDs));}

Order Fifo::createSpin(uint8_t nerv,float thetaAim,uint8_t timeoutDs)
{
  Order ord;
  ord.type=SPIN_TYPE;
  ord.spin.nerv=nerv;
  ord.spin.thetaAim=thetaAim;
  ord.timeoutDs=timeoutDs;
  return(ord);
}
void Fifo::addSpin(uint8_t nerv,float thetaAim,uint8_t timeoutDs)
{add(createSpin(nerv,thetaAim,timeoutDs));}

Order Fifo::createFWD(float acc,float v,uint8_t timeoutDs)
{
  Order ord;
  ord.type=FWD_TYPE;
  ord.fwd.acc=acc;
  ord.fwd.v=v;
  ord.timeoutDs=timeoutDs;
  return(ord);
}
void Fifo::addFWD(float acc,float v,uint8_t timeoutDs)
{add(createFWD(acc,v,timeoutDs));}

Order Fifo::createBWD(float acc,float v,uint8_t timeoutDs)
{
  Order ord;
  ord.type=BWD_TYPE;
  ord.bwd.acc=acc;
  ord.bwd.v=v;
  ord.timeoutDs=timeoutDs;
  return(ord);
}
void Fifo::addBWD(float acc,float v,uint8_t timeoutDs)
{add(createBWD(acc,v,timeoutDs));}

Order Fifo::createSTBY(uint8_t nerv,const char unlockMessage[],uint8_t timeoutDs)
{
  Order ord;
  ord.type=STBY_TYPE;
  ord.stby.nerv=nerv;
  strSet(ord.stby.unlockMessage,unlockMessage);
  ord.timeoutDs=timeoutDs;
  return(ord);
}
void Fifo::addSTBY(uint8_t nerv,const char unlockMessage[],uint8_t timeoutDs)
{add(createSTBY(nerv,unlockMessage,timeoutDs));}

Order Fifo::createEmStop(uint8_t timeoutDs)
{
  Order ord;
  ord.type=EMSTOP_TYPE;
  ord.timeoutDs=timeoutDs;
  return(ord);
}
void Fifo::addEmStop(uint8_t timeoutDs)
{add(createEmStop(timeoutDs));}

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
