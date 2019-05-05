#ifndef COMM_INCLUDED
#define COMM_INCLUDED
#include "Arduino.h"
#include "1_CONSTANT.h"
struct VectorE;
class Comm
{
public:
    MessageE lastMessage=MessageE::Default;
    float collisionX,collisionY;
    void actuate(VectorE posERobot,float vRobot);
    void taken();
    void send(MessageE message);
    Comm();
private:
    enum StatesE{Standard,WaitingX,WaitingY} state;
    uint32_t millisLastSendPos,millisOutOfStandardState;
    void send(uint8_t value);
};
#endif
