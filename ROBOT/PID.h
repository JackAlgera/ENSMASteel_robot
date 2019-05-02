#ifndef PID_INCLUDED
#define PID_INCLUDED

#include "1_CONSTANT.h"
#include "Arduino.h"
#include "Fifo.h"
#include "Moteur.h"
#include "Ghost.h"
#include "Cerveau.h"

/*
En mode RUSH:
Les Goto doivent être des lignes droites.
Les Spin marchent niquel

En mode STD: (STANDARD)
Ca marche bien, les legeres courbes sont autorisées.

En mode ACRT: (ACCURATE)
Toute les courbes sont possibles. Vitesse lente. Le I est assez elevé ce qui permet de passer les obstacles

En mode DYDM: (DON'T YOU DARE MOVE)
Beaucoup de I. C'est le mode statique, verrouillé sur place
 */

class PID
{
    uint8_t PIDnervLIN=DYDM,PIDnervANG=DYDM;
    VectorE lastPosERobot= {{0,0},0};
    float lastVRobot=0,lastDt=1/FREQUENCY;
    float IL=0.0,IA=0.0;
    Robot * ptrRobot;
public:

    void actuate(float dt,VectorE posERobot,float vRobot,float wRobot);              //Actualise les PID (compare la position du ghost et du robot, et donne les ordres au moteur en fonction
    void reload();
    void loadNext();

    PID();
    PID(Robot * ptrRobot);
    ~PID();
};

#endif
