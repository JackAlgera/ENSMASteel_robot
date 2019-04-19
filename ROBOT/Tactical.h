#include "Fifo.h"
#include "Vector.h"

enum Actions {  Chaos, Distribx6, 3depart, Distribx3, Rampe,
                RecupBlueAcc, PoseAcc, RecupeGoldAcc, Balance, PoseSol };   // On aura Chaos = 0, Distribx6 = 1 etc..
                                                                            // Actions::Chaos pour y acceder -> exemple : if(!DONE[Actions::Chaos]) {...}

bool DONE[10];
