#ifndef CONSTANT_INCLUDED
#define CONSTANT_INCLUDED


//-------------------------MODE DE FONCTIONNEMENT-------------------------
//#define RECORD                    //Permet d'activer ou non le feedback
//#define RC
//#define FILTRE_SETUP_ANG
//#define FILTRE_SETUP_LIN
//#define W_TUNE
//#define V_TUNE
//#define THETA_TUNE
//#define DELAY_TUNE
//#define X_TUNE
#define STATIQUE false  //Permet de faire des essais sans robot
#define PIDL false
#define PIDA true

//-------------------------PINS-------------------------
#define PIN_CODEUSE_GAUCHE_A 3
#define PIN_CODEUSE_GAUCHE_B 5
#define PIN_CODEUSE_DROITE_A 2
#define PIN_CODEUSE_DROITE_B 4

#define PIN_MOTEUR_GAUCHE_PWR 10
#define PIN_MOTEUR_GAUCHE_SENS1 6
#define PIN_MOTEUR_GAUCHE_SENS2 7

#define PIN_MOTEUR_DROITE_PWR 9
#define PIN_MOTEUR_DROITE_SENS1 11
#define PIN_MOTEUR_DROITE_SENS2 8

#define MPUADDRESS 0x68

//-------------------------PHYSIQUE-------------------------
#define ELOIGNEMENT_CODEUSES 0.4008
#define DIAMETRE_ROUE_CODEUSE 0.09
#define TICKS_PER_ROUND 1200.0
#define MAXPWM 1023

//-------------------------VOCABULAIRE-------------------------
#define GAUCHE 0
#define DROITE 1


#define ACRT 0  //accurate
#define STD 1   //standard
#define RUSH 2  //You know what I mean
#define DYDM 3 //Don't you dare move
#define OFF 4 //Pas de PID

#define LIN 0
#define ANG 1

#define KP 0
#define KI 1
#define KD 2

#define GOTO_TYPE 0             //Le goto deplace le robot a l'endroit selectionné depuis l'endroit où il est au moment où il execute l'action
#define SPIN_TYPE 1             //Le spin fait tourner le robot sur place depuis l'angle qu'il a au moment où il execute l'action
#define FWD_TYPE 2              //Le FWD fait avancer le robot jusqu'a la modification d'une variable boolean
#define BWD_TYPE 3              //Le FWD fait avancer le robot en marche arrière jusqu'a la modification d'une variable boolean
#define STBY_TYPE 4             //Le Standy est un mode statique qui se débloque a la fin d'un timer ou lorsque quelquechose passe la variable locked a false
#define SEND_TYPE 5

//-------------------------DEPLACEMENTS-------------------------

#define RAYON_RECONVERGENCE 0.90             //Si le robot s'est trop eloigné (plus que RAYON_RECONVERGENCE), il va se tourner de façon a rejoindre le ghost
#define RAYON_TERMINE 0.05                //Distance en dessous duquel on considère que le robot est arrivé
#define DELTA_THETA_TERMINE 0.08               //Delta theta en dessous duquel on considère le spin terminé 
#define FREQUENCY 50.0
#define DELAY 0.100
//NERV ---> FIFO.CPP


//-------------------------MISCENCELLOUS-------------------------
#define TAILLEFIFO 8           //Taille du buffer circulaire
#define DEGRE_MAX 6

#endif
