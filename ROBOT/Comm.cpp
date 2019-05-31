//#include "Comm.h"
//#include "Arduino.h"
//#include "Vector.h"
//
//void Comm::actuate(VectorE posERobot,AnticolE anticol)
//{
//    if (state!=StatesE::Standard and (millis()-millisOutOfStandardState)>500)state=StatesE::Standard;
//    if (Serial1.available()>=1)
//    {
//        switch(state)
//        {
//            case StatesE::Standard:
//                lastMessage=(MessageE)Serial1.read();
//                if((uint8_t)lastMessage>=NB_MESSAGES){lastMessage=MessageE::Default;Serial.print("On a recu de le merde");} //On a recu de la merde
//                if (lastMessage==MessageE::Evitemment)
//                    {
//                        if (millis()-millisLastEvitemment>500)
//                        {
//                            lastMessage=MessageE::Default;  //Les informations ne sont pas encore prete a etre lues
//                            state=StatesE::WaitingX;
//                            millisOutOfStandardState=millis();
//                        }
//                        else
//                        {
//                            lastMessage=MessageE::Default;
//                        }
//                    }
//            break;
//            case StatesE::WaitingX:
//                collisionX=((float)Serial1.read())*3.0/255.0;  //Produit en croix
//                state=StatesE::WaitingY;
//            break;
//            case StatesE::WaitingY:
//                collisionY=((float)Serial1.read())*2.0/255.0;  //Produit en croix
//                state=StatesE::Standard;
//                lastMessage=MessageE::Evitemment;  //Les informations sont pretes
//                #ifdef STATE
//                Serial.println("message d'evitemment recu");
//                Serial.print("X : ");Serial.println(collisionX);
//                Serial.print("Y : ");Serial.println(collisionY);
//                #endif
//                millisLastEvitemment=millis();
//            break;
//        }
//
//        #ifdef STATE
//            Serial.print("lastMessage ");Serial.println((uint8_t)lastMessage);
//        #endif
//    }
//    if (millis()-millisLastSendPos>500)
//    {
//      millisLastSendPos=millis();
//      send(MessageE::Sync);
//      send((uint8_t)(posERobot.vec.x*255/3.0));
//      send((uint8_t)(posERobot.vec.y*255/2.0));
//      send((uint8_t)((posERobot.theta+PI)*255/(2*PI)));
//      send((uint8_t)(anticol));
//    }
//
//}
//



#include "Comm.h"
#include "Arduino.h"
#include "Vector.h"

void Comm::actuate(VectorE posERobot,AnticolE anticol)
{
    if (state!=StatesE::Standard and (millis()-millisOutOfStandardState)>500)state=StatesE::Standard;
    if (Serial1.available()>=1)
    {
                lastMessage=(MessageE)Serial1.read();
                if((uint8_t)lastMessage>=NB_MESSAGES)
                {
                    lastMessage=MessageE::Default;
                    Serial.print("On a recu de le merde"); //On a recu de la merde
                }
                if(lastMessage==MessageE::Tirette)
                    tStartMatch=millis()/1000.0;


        #ifdef STATE
            Serial.print("lastMessage ");Serial.println((uint8_t)lastMessage);
        #endif
    }
    if (millis()-millisLastSendPos>500)
    {
      millisLastSendPos=millis();
      send(MessageE::Sync);
      send((uint8_t)(posERobot.vec.x*255/3.0));
      send((uint8_t)(posERobot.vec.y*255/2.0));
      send((uint8_t)((posERobot.theta+PI)*255/(2*PI)));
      send((uint8_t)(anticol));
    }
    if (millis()/1000.0-(tStartMatch+100)>0.0)
        lastMessage=MessageE::Em_stop;

}

void Comm::taken()
{
    lastMessage=MessageE::Default;
}

void Comm::send(MessageE message)
{
    Serial1.write((uint8_t)message);
}

void Comm::send(uint8_t value)
{
    Serial1.write(value);
}

Comm::Comm()
{
  while(Serial.available()>0){Serial.read();}
  state=StatesE::Standard;
  millisLastSendPos=millis();
  millisLastEvitemment=0;
}

