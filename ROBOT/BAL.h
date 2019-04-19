#ifndef BAL_INCLUDED
#define BAL_INCLUDED

bool strEqual(char str1[],char str2[]);
void strSet(char *str,const char in[]);

class BAL
{
  public:
  char lastMessage[4]="OBS";
  void actuate();
  void taken();
};

#endif
