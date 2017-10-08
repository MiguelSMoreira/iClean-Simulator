#ifndef READFILE_H_
#define READFILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structs.h"
#include "ListMANIP.h"

int WriteFILE(MapDiv** , Frota*);
Pointers* ReadFILE(char*);
//int ReadXY(char* filename);
MapDiv** DynPointerARRAY(int, int);
Frota* ListALLOC(int);

#endif // READFILE_H_
