#ifndef COMM_INCLUDED
#define COMM_INCLUDED
#include "Arduino.h"
#include "1_CONSTANT.h"
struct VectorE;
class Comm
{
public:

    MessageE lastMessage=MessageE::Default;
    float tStartMatch=90000;
    float collisionX,collisionY;
    void actuate(VectorE posERobot,AnticolE anticol);
    void taken();
    void send(MessageE message);
    Comm();
private:
    enum StatesE{Standard,WaitingX,WaitingY} state;
    uint32_t millisLastSendPos,millisOutOfStandardState,millisLastEvitemment;
    void send(uint8_t value);
};
#endif
