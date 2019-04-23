#include "Comm.h"
#include "Arduino.h"

bool strEqual(char* str1,char* str2)
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


void Comm::actuate()
{
  if (Serial.available()>0 && Serial.peek()!='!'){Serial.read();}
  if (Serial.available()>=5 && Serial.peek()=='!')
  {
    Serial.read(); //On retire le !
    lastMessage[0]=Serial.read();
    lastMessage[1]=Serial.read();
    lastMessage[2]=Serial.read();
    lastMessage[3]=Serial.read();
  }
  //Serial.print(Serial.available());Serial.print(lastMessage[0]);Serial.print(lastMessage[1]);Serial.print(lastMessage[2]);Serial.println(lastMessage[3]);
}

void Comm::taken()
{
  strSet(lastMessage,"OBSL");
}
