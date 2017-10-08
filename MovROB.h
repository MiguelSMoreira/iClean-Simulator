#ifndef MOVROB_H_
#define MOVROB_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Structs.h"
#include "SDL.h"

int VerifCLEAN(MapDiv**, int, int);
void CalcPos(Frota*);
float RandomFloat(float, float);
void MovROB(Frota*, MapDiv**, int, int, int, int, float);

#endif // MOVROB_H_
