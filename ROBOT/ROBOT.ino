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
#include "Robot.h"

void print7(float f1,float f2,float f3,float f4,float f5,float f6,float f7)
{
    Serial.print(f1);
    Serial.print(";");
    Serial.print(f2);
    Serial.print(";");
    Serial.print(f3);
    Serial.print(";");
    Serial.print(f4);
    Serial.print(";");
    Serial.print(f5);
    Serial.print(";");
    Serial.print(f6);
    Serial.print(";");
    Serial.print(f7);
    Serial.print("!");
}

void printRobotState(Robot* robot)
{
#ifdef RECORD
    print7( micros()/1000.0, robot->posE.vec.x*10000, robot->posE.vec.y*10000, robot->posE.theta*10000, robot->ghost.posED.vec.x*10000, robot->ghost.posED.vec.y*10000, robot->ghost.posED.theta*10000);
#endif
#ifdef FILTRE_SETUP_ANG
    print7( micros()/1000.0, robot->wF.out()*10000, robot->wF.raw*10000, robot->ghost.w*10000, 0, 0,0);
#endif
#ifdef FILTRE_SETUP_LIN
    print7( micros()/1000.0, robot->vF.out()*10000, robot->vF.raw*10000, robot->ghost.v*10000, 0, 0,0);
#endif
#ifdef W_TUNE
    print7( micros()/1000.0, robot->wF.out()*10000, robot->ghost.w*10000, 0, 0, 0,0);
#endif
#ifdef V_TUNE
    print7( micros()/1000.0, robot->vF.out()*10000, robot->ghost.v*10000, 0, 0, 0,0);
#endif
#ifdef THETA_TUNE
    print7( micros()/1000.0, robot->posE.theta*10000, robot->ghost.posED.theta*10000, 0, 0, 0,0);
#endif
#ifdef X_TUNE
    print7( micros()/1000.0, robot->posE.vec.x*10000, robot->ghost.posED.vec.x*10000, 0, 0, 0,0);
#endif
#ifdef DELAY_TUNE
    print7( micros()/1000.0, robot->ghost.posED.vec.x*10000, robot->ghost.posED.vec.y*10000, robot->ghost.posED.theta*10000, robot->ghost.posE.vec.x*10000, robot->ghost.posE.vec.y*10000, robot->ghost.posE.theta*10000);
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------- Programme principal -------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------

uint32_t m, microsStart, mLoop;
float dtLoop;
Robot robot;
//Cerveau master;

void setup()
{
    delay(5000);
    Serial.begin(115200);
    Serial.println("--REBOOT--");
    robot.set(1.5,1.0,0.0);
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
