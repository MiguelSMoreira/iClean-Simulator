#ifndef LISTMANIP_H_
#define LISTMANIP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Structs.h"
#include "MovROB.h"
#include "ReadFILE.h"

void FillLIST(Frota*, int, int, int, int, char*, MapDiv**, float);
Frota* AddROBOT(Frota*, int , int, int , int, char*, MapDiv**, float, int*);
Frota* RemROBOT(Frota*, MapDiv**);
void EraseLIST(Frota*);
void EraseMAPS(MapDiv**, int);

#endif // LISTMANIP_H_
