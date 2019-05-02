#include "Comm.h"
#include "Arduino.h"
#include "Fifo.h"
#include "ContreMesure.h"

bool strEqual(char* str1,char* str2)
{
    return (str1[0]==str2[0] && str1[1]==str2[1] && str1[2]==str2[2] && str1[3]==str2[3]);
}

bool strEqual(const char str1[],char* str2)
{
    return (str1[0]==str2[0] && str1[1]==str2[1] && str1[2]==str2[2] && str1[3]==str2[3]);
}

bool strEqual(char* str1,const char str2[])
{
    return (str1[0]==str2[0] && str1[1]==str2[1] && str1[2]==str2[2] && str1[3]==str2[3]);
}


void strSet(char *str,const char in[])
{
    str[0]=in[0];
    str[1]=in[1];
    str[2]=in[2];
    str[3]=in[3];
}

void Comm::set(Fifo* in_ordresRobot,PID* in_ptrPid)
{
    ordresRobot = in_ordresRobot;
    ptrPid = in_ptrPid;
}

void Comm::specialBehavior()
{
    char special1[]="ESTP"; //Emergency SToP
    char special2[]="BACK";
    char special3[]="FAIL";
    if (strEqual(special1,lastMessage))
    {
        taken();
        ordresRobot->addHead(STBY(OFF, "DUMY", 100,nullptr,normalTimeout,0));
        ordresRobot->addHead(EMSTOP(10,nullptr,simpleTimeout,0));
        ptrPid->reload();
    }
    if (strEqual(special2,lastMessage))
    {
        taken();
        ordresRobot->add(SPINGOTO(RUSH,0,0,50,nullptr,simpleTimeout,0));
    }
    if (strEqual(special3,lastMessage))
    {
        taken();
        ptrPid->failureDetected(ErreurE::MEGA);
    }
}

void Comm::actuate()
{
    if (Serial.available()>0 && Serial.peek()!='!')
    {
        Serial.read();
    }
    if (Serial.available()>=5 && Serial.peek()=='!')
    {
        Serial.read(); //On retire le !
        lastMessage[0]=Serial.read();
        lastMessage[1]=Serial.read();
        lastMessage[2]=Serial.read();
        lastMessage[3]=Serial.read();
#ifdef STATE
        Serial.print(lastMessage[0]);
        Serial.print(lastMessage[1]);
        Serial.print(lastMessage[2]);
        Serial.println(lastMessage[3]);
#endif
    }

    specialBehavior();
}

void Comm::taken()
{
    strSet(lastMessage,"OBSL");
}

void Comm::send(const char message[])
{
    for (int i=0; i<=3; i++)
        Serial.print(message[i]);
    Serial.println();
}
