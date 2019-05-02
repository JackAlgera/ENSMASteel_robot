#ifndef COMM_INCLUDED
#define COMM_INCLUDED
#include "Fifo.h"
#include "PID.h"

bool strEqual(char *str1,char *str2);
bool strEqual(const char str1[],char *str2);
bool strEqual(char *str1,const char str2[]);
void strSet(char *str,const char in[]);

class Comm
{
public:
    Fifo* ordresRobot;
    PID* ptrPid;
    char lastMessage[4]="OBS";
    void actuate();
    void taken();
    void set(Fifo* in_ordresRobot,PID* in_ptrPid);
    void send(const char message[]);
private:
    void specialBehavior();
};
#endif
