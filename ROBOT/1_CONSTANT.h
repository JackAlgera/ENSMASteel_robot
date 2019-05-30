#ifndef CONSTANT_INCLUDED
#define CONSTANT_INCLUDED


//-------------------------MODE DE FONCTIONNEMENT-------------------------
#define RECORD  //Permet d'activer ou non le feedback
//#define STATE
//#define RC
//#define FILTRE_SETUP_ANG
//#define FILTRE_SETUP_LIN
//#define W_TUNE
//#define V_TUNE
//#define THETA_TUNE
//#define DELAY_TUNE
//#define X_TUNE
#define AVOIDANCE true
#define STATIQUE false            //Permet de faire des essais sans robot


//-------------------------PINS-------------------------
#define PIN_CODEUSE_GAUCHE_A 29
#define PIN_CODEUSE_GAUCHE_B 28
#define PIN_CODEUSE_DROITE_A 34
#define PIN_CODEUSE_DROITE_B 35

#define PIN_MOTEUR_GAUCHE_PWR 22
#define PIN_MOTEUR_GAUCHE_SENS 36
#define PIN_MOTEUR_GAUCHE_BRAKE 37

#define PIN_MOTEUR_DROITE_PWR 23
#define PIN_MOTEUR_DROITE_SENS 38
#define PIN_MOTEUR_DROITE_BRAKE 39

#define PIN_CONTACTEUR_GAUCHE 30
#define PIN_CONTACTEUR_DROITE 33


//-------------------------PHYSIQUE-------------------------
//#define ELOIGNEMENT_CODEUSES 0.296255
#define ELOIGNEMENT_CODEUSES 0.29430415
#define DIAMETRE_ROUE_CODEUSE_DROITE 0.05325315
#define DIAMETRE_ROUE_CODEUSE_GAUCHE 0.053570956
#define TICKS_PER_ROUND 16384
#define MAXPWM 4090
#define RROBOT 0.20
#define HROBOT 0.1365

//-------------------------VOCABULAIRE-------------------------
#define GAUCHE 0
#define DROITE 1

#define NERV RUSH
#define TMOUT 100

#define RECALLE 0    //recalle
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
    GOTO_E, SPIN_E, SPINGOTO_E, SPINTO_E, GO_UNTIL_E, STBY_E, SEND_E, EMSTOP_E, SETX_E,SETY_E
};

#define NB_MESSAGES 17
enum MessageE
{
    Default,Impossible, Tirette, Pince_Retracted,Pince_Half_Retracted,Pince_Half_Extended,Pince_Extended, Evitemment, Ok, Done, New_Action, Sync, Evitemment_Clear, Calle,Violet,Jaune,Start_Chaos
};

enum AnticolE{Front,Back,No};

#define NB_ACTIONS 14
enum ActionE
{
    Start, Chaos, Distribx6_1, Distribx6_2, Distribx6_3, CoupDeCul, Distribx3_1, Distribx3_2, RecupBlueAcc, PoseAcc, RecupGoldAcc, Balance, PoseSol, CasseCouilles
};

enum ErreurE
{
    TIMEOUT,PID_FAILURE,MEGA
};

//-------------------------DEPLACEMENTS-------------------------

#define RAYON_RECONVERGENCE 10.5          //Si le robot s'est trop eloigné (plus que RAYON_RECONVERGENCE), il va se tourner de façon a rejoindre le ghost
#define RAYON_TERMINE 0.002                //Distance en dessous duquel on considère que le robot est arrivé
#define DELTA_THETA_TERMINE 0.012
//#define DELTA_THETA_TERMINE 0.017          //Delta theta en dessous duquel on considère le spin terminé

#define RAYON_FAIL 0.10
#define DELTA_THETA_FAIL 0.5
#define MAX_W 0.012
#define FAIL_TIME 0.5

#define FREQUENCY 200.0
#define DELAY 0.050
#define DISTANCE_EVITEMMENT 0.10
//NERV ---> FIFO.CPP


//-------------------------MISCENCELLOUS-------------------------
#define TAILLEFIFO 40           //Taille du buffer circulaire
#define DEGRE_MAX 6


#endif
