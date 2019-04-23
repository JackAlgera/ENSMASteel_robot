#ifndef COMM_INCLUDED
#define COMM_INCLUDED

bool ptrEqualStr(char *str1,char str2[]);
void ptrSetStr(char *str,const char in[]);

class COMM
{
  public:
  char lastMessage[5]="OBSL";
  void actuate();
  void taken();
};

#endif
