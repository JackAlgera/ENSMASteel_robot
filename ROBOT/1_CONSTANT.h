#ifndef CONSTANT_INCLUDED
#define CONSTANT_INCLUDED


//-------------------------MODE DE FONCTIONNEMENT-------------------------
//#define RECORD  //Permet d'activer ou non le feedback
#define STATE
//#define RC
//#define FILTRE_SETUP_ANG
//#define FILTRE_SETUP_LIN
//#define W_TUNE
//#define V_TUNE
//#define THETA_TUNE
//#define DELAY_TUNE
//#define X_TUNE
#define STATIQUE false            //Permet de faire des essais sans robot
#define PIDL true
#define PIDA true

//-------------------------PINS-------------------------
#define PIN_CODEUSE_GAUCHE_A 28
#define PIN_CODEUSE_GAUCHE_B 29
#define PIN_CODEUSE_DROITE_A 34
#define PIN_CODEUSE_DROITE_B 35

#define PIN_MOTEUR_GAUCHE_PWR 22
#define PIN_MOTEUR_GAUCHE_SENS1 36
#define PIN_MOTEUR_GAUCHE_SENS2 37

#define PIN_MOTEUR_DROITE_PWR 23
#define PIN_MOTEUR_DROITE_SENS1 38
#define PIN_MOTEUR_DROITE_SENS2 39

#define PIN_CONTACTEUR_GAUCHE 30
#define PIN_CONTACTEUR_DROITE 33

//-------------------------PHYSIQUE-------------------------
#define ELOIGNEMENT_CODEUSES 0.4008
#define DIAMETRE_ROUE_CODEUSE 0.09
#define TICKS_PER_ROUND 1200.0
#define MAXPWM 65535

//-------------------------VOCABULAIRE-------------------------
#define GAUCHE 0
#define DROITE 1

#define NERV RUSH
#define TMOUT 100

#define ACRT 0    //accurate
#define STD 1     //standard
#define RUSH 2    //You know what I mean
#define DYDM 3    //Don't you dare move
#define OFF 4     //Pas de PID

#define LIN 0
#define ANG 1

#define KP 0
#define KI 1
#define KD 2

enum OrderE
{
    GOTO_E, SPIN_E, SPINGOTO_E, FWD_E, BWD_E, STBY_E, SEND_E, EMSTOP_E
};

// On aura Chaos = 0, Distribx6 = 1 etc..
// ActionE::Chaos pour y acceder -> exemple : if(!DONE[ActionE::Chaos]) {...}
#define NBR_ACTIONS 15


enum ActionE
{
    Chaos, Distribx6_1, Distribx6_2, Distribx6_3, CoupDeCul, Distribx3, MonteRampe, PoseRampe, DescendRampe, RecupBlueAcc, PoseAcc, RecupGoldAcc, Balance, PoseSol, CasseCouilles
};

enum ErreurE
{
    FAIL
};

//-------------------------DEPLACEMENTS-------------------------

#define RAYON_RECONVERGENCE 0.90          //Si le robot s'est trop eloigné (plus que RAYON_RECONVERGENCE), il va se tourner de façon a rejoindre le ghost
#define RAYON_TERMINE 0.05                //Distance en dessous duquel on considère que le robot est arrivé
#define DELTA_THETA_TERMINE 0.08          //Delta theta en dessous duquel on considère le spin terminé
#define FREQUENCY 50.0
#define DELAY 0.100
//NERV ---> FIFO.CPP


//-------------------------MISCENCELLOUS-------------------------
#define TAILLEFIFO 40           //Taille du buffer circulaire
#define DEGRE_MAX 6

#endif
