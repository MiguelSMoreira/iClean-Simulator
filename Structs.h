#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct MapaDivisao
{
    int est;
    int ocupied;
    int dest;
    char name[3];
} MapDiv;

typedef struct FrotaRobots
{
    char name[3];
    int x;
    int y;
    int x_seg;
    int y_seg;
    float bat;
    int qd_perc;
    int qd_limpos;
    MapDiv** mapa;
    struct FrotaRobots *seg;
    struct FrotaRobots *ant;
} Frota;

typedef struct ReadPointers
{
    Frota* frota;
    MapDiv** mapa;
    int x;
    int y;
    int a;
    int oc;
} Pointers;

#endif // STRUCTS_H_
