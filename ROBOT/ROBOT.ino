#include "1_CONSTANT.h"
#include "Moteur.h"
#include "Codeuse.h"
#include "Fifo.h"
#include "Moteur.h"
#include "PID.h"
#include "Polynome.h"
#include "Vector.h"
#include "Arduino.h"
#include "Filtre.h"
#include "Comm.h"
#include "Cerveau.h"
#include "Encoder.h"

class Robot
{
  public:
    float dt;
    Motor moteurGauche, moteurDroite;
    Codeuse codeuseGauche,codeuseDroite;
    Ghost ghost;
    VectorE posE;                                 //Position du robot
    Filtre vF,wF;
    Fifo ordresFifo;                              //Une liste d'ordres a effectuer
    PID pid;
    Comm comm;
    Cerveau master;

    void actuateODO(float dt);
    void set(float x0,float y0, float theta0);    //Remplie les champs de l'objet Robot
    void actuate(float dt);                       //Actualise les valeurs
};

void Robot::actuate(float dt)
{
  codeuseGauche.actuate(dt);
  codeuseDroite.actuate(dt);
  actuateODO(dt);
  ghost.actuate(dt);
  pid.actuate(dt,posE,vF.out(),wF.out());
  comm.actuate();
  master.actuate();
}

void Robot::actuateODO(float dt)
{
  vF.in((codeuseDroite.v+codeuseGauche.v)/2.0,dt);
  wF.in((codeuseDroite.v-codeuseGauche.v)/ELOIGNEMENT_CODEUSES,dt);
  posE.theta = normalize(posE.theta+(codeuseDroite.deltaAvance-codeuseGauche.deltaAvance)/ELOIGNEMENT_CODEUSES);
  float avance = (codeuseDroite.deltaAvance+codeuseGauche.deltaAvance)/2;
  posE.vec.x += avance*cos(posE.theta);
  posE.vec.y += avance*sin(posE.theta);  
}


void Robot::set(float x0,float y0, float theta0)
{
  delay(1000);
  moteurDroite  = init_motor(PIN_MOTEUR_DROITE_PWR,PIN_MOTEUR_DROITE_SENS1,PIN_MOTEUR_DROITE_SENS2,1.0);
  moteurGauche  = init_motor(PIN_MOTEUR_GAUCHE_PWR,PIN_MOTEUR_GAUCHE_SENS1,PIN_MOTEUR_GAUCHE_SENS2,0.96);
  codeuseGauche = Codeuse(GAUCHE, new Encoder(PIN_CODEUSE_GAUCHE_A,PIN_CODEUSE_GAUCHE_B));
  codeuseDroite = Codeuse(DROITE, new Encoder(PIN_CODEUSE_DROITE_A,PIN_CODEUSE_DROITE_B));
  VectorE initVect = init_vectorE(x0,y0,theta0);
  ghost = *(new Ghost(initVect));
  posE.vec.x = x0;posE.vec.y=y0;posE.theta=theta0;
  vF = newFiltre(0.0,60.0,2);wF=newFiltre(0.0,60.0,2);
  ordresFifo.add(STBY(DYDM,"Tirt",255,nullptr));
  master=*(new Cerveau(&ordresFifo));
  pid = PID(&moteurGauche,&moteurDroite,&ordresFifo,&ghost,&comm,&master);
  comm.set(&ordresFifo,&pid);
}

//Au cas ou....
void emergencyStop(Robot* r)
{
  r->ordresFifo.clean();
  r->moteurGauche.order=0;
  r->moteurDroite.order=0;
  r->moteurGauche.actuate();
  r->moteurDroite.actuate();
}

void print7(float f1,float f2,float f3,float f4,float f5,float f6,float f7)
{
  Serial.print(f1);Serial.print(";");
  Serial.print(f2);Serial.print(";");
  Serial.print(f3);Serial.print(";");
  Serial.print(f4);Serial.print(";");
  Serial.print(f5);Serial.print(";");
  Serial.print(f6);Serial.print(";");
  Serial.print(f7);Serial.print("!");
}

void printRobotState(Robot* robot)
{
    #ifdef RECORD
      print7( micros()/1000.0 , robot->posE.vec.x*10000 , robot->posE.vec.y*10000 , robot->posE.theta*10000 , robot->ghost.posED.vec.x*10000 , robot->ghost.posED.vec.y*10000 , robot->ghost.posED.theta*10000);
    #endif
    #ifdef FILTRE_SETUP_ANG
      print7( micros()/1000.0 , robot->wF.out()*10000, robot->wF.raw*10000 , robot->ghost.w*10000 , 0 , 0 ,0);
    #endif
    #ifdef FILTRE_SETUP_LIN
      print7( micros()/1000.0 , robot->vF.out()*10000, robot->vF.raw*10000 , robot->ghost.v*10000 , 0 , 0 ,0);
    #endif
    #ifdef W_TUNE
      print7( micros()/1000.0 , robot->wF.out()*10000, robot->ghost.w*10000 , 0 , 0 , 0 ,0);
    #endif
    #ifdef V_TUNE
      print7( micros()/1000.0 , robot->vF.out()*10000, robot->ghost.v*10000 , 0 , 0 , 0 ,0);
    #endif
    #ifdef THETA_TUNE
      print7( micros()/1000.0 , robot->posE.theta*10000, robot->ghost.posED.theta*10000 , 0 , 0 , 0 ,0);
    #endif
    #ifdef X_TUNE
      print7( micros()/1000.0 , robot->posE.vec.x*10000, robot->ghost.posED.vec.x*10000 , 0 , 0 , 0 ,0);
    #endif
    #ifdef DELAY_TUNE
      print7( micros()/1000.0 , robot->ghost.posED.vec.x*10000, robot->ghost.posED.vec.y*10000 , robot->ghost.posED.theta*10000 , robot->ghost.posE.vec.x*10000, robot->ghost.posE.vec.y*10000 , robot->ghost.posE.theta*10000);
    #endif
}

// ----------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------- Programme principal --------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------

uint32_t m, microsStart, mLoop;
float dtLoop;
Robot robot;
Cerveau master;

void setup()
{   
  delay(5000);
  Serial.begin(115200);
  robot.set(1.5,1.0,0.0);				// Malheureusement je set le master ici aussi pour l'instant
  Serial.println("--REBOOT--");
  m=micros();
  microsStart=m-((uint32_t)(1.0/FREQUENCY*1000000.0));
}

void loop()
{
  m=micros();
  dtLoop=(m-microsStart)/1000000.0;
  microsStart=m;
  robot.ordresFifo.printAll();
  robot.actuate(dtLoop);
  printRobotState(&robot);
 
  while((micros()-microsStart)/1000000.0<1.0/FREQUENCY) {;}
}
