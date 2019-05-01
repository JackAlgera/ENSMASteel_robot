#ifndef GHOST_INCLUDED
#define GHOST_INCLUDED

#include "1_CONSTANT.h"
#include "Polynome.h"
#include "Vector.h"
#include "stdint.h"
#include "Filtre.h"

class Ghost
{
  public: 
    float t=0.0,t_e=0.0,v_e=1.0;                            // Temps depuis init,Temps fictif (entre 0 et 1), vitesse fictive
    bool spinning=true,reversed=false,locked=true;
    uint32_t microsStart=0;                                 // Microsecondes ecoulée lors du dernier appel
    Polynome X_P,Y_P,v_e_P2,DDX_P,DDY_P;                    // X en fonction de t*; Y en fonction de t*; Ce sont des chemins!!!!
    S_function s;                                           // Donne l'abcisse curviligne en fonction du temps
    S_function theta_S;                                     // Donne theta en fonction du temps
    VectorE posE,posED;                                     // Postion du fantome
    Filtre fDelayX,fDelayY,fDelayTheta,wF;
    float v,w;                                              // Vitesse et vitesse angulaire du fantome

    Ghost();
    Ghost(VectorE posEini);
    void recalle(VectorE posERobot, float v);               // Teleporte le fantome sur le robot
    void actuate(float dt);                                 // Actualise la position grace aux polynomes 
    void unlock();
};
#endif
