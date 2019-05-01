#include "Fifo.h"
#include "Vector.h"
#include "Comm.h"

Fifo::Fifo()
{
}

Fifo::~Fifo()
{
}

void Fifo::add(Order order)
{
    indiceFin = (indiceFin+1)%TAILLEFIFO;
    inBuffer++;
    liste[indiceFin] = order;
}

void Fifo::addHead(Order order)
{
    indiceDebut = (indiceDebut-1+TAILLEFIFO)%TAILLEFIFO;
    inBuffer++;
    liste[indiceDebut] = order;
}

void Fifo::replaceHead(Order order)
{
    liste[indiceDebut] = order;
}

void Fifo::pop()
{
    indiceDebut = (indiceDebut + 1) % TAILLEFIFO;
    inBuffer--;
}

void Fifo::printAll()
{
    int i=indiceDebut;
    while(i!=indiceFin)
    {
        Serial.print(liste[i].type);
        Serial.print(" ");
        i=(i+1)%TAILLEFIFO;
    }
    Serial.print(liste[i].type);
    Serial.print(" indice Debut ");
    Serial.print(indiceDebut);
    Serial.print(" indice Fin ");
    Serial.println(indiceFin);
}

Order* Fifo::ptrFst()
{
    return &(liste[(indiceDebut) % TAILLEFIFO]);
}

void Fifo::clean()
{
    indiceDebut = 1;
    indiceFin = 0;
    inBuffer = 0;
    add(STBY(DYDM, "DUMY", 255,nullptr));
}
