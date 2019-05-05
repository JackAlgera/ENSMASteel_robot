#include "Comm.h"
#include "Arduino.h"
#include "Vector.h"

void Comm::actuate(VectorE posERobot,float vRobot)
{
    if (Serial1.available()>=1)
    {
        switch(state)
        {
            case StatesE::Standard:
                lastMessage=(MessageE)Serial1.read();
                //if((uint8_t)lastMessage>=NB_MESSAGES){lastMessage=MessageE::Default;Serial.print("On a recu de le merde");} //On a recu de la merde
                if (lastMessage==MessageE::Evitemment)
                    {
                        lastMessage=MessageE::Default;  //Les informations ne sont pas encore prete a etre lues
                        state=StatesE::WaitingX;
                    }
            break;
            case StatesE::WaitingX:
                collisionX=((float)Serial1.read())*3.0/255.0;  //Produit en croix
                state=StatesE::WaitingY;
            break;
            case StatesE::WaitingY:
                collisionY=((float)Serial1.read())*2.0/255.0;  //Produit en croix
                state=StatesE::Standard;
                lastMessage=MessageE::Evitemment;  //Les informations sont pretes
                #ifdef STATE
                Serial.println("message d'evitemment recu");
                Serial.print("X : ");Serial.println(collisionX);
                Serial.print("Y : ");Serial.println(collisionY);
                #endif
            break;
        }
        
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
      send((uint8_t)(vRobot>0));
    }

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
}
