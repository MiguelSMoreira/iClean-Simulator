#ifndef SDL_H_
#define SDL_H_

#define STRING_SIZE 100
#define MAP_SIZE 1000
#define WINDOW_POSX 500
#define WINDOW_POSY 250
#define SQUARE_SIZE 30
#define EXTRASPACE 150
#define MARGIN 4

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Structs.h"
#include "ListMANIP.h"
#include "ReadFILE.h"

void DrawLine(Frota*, SDL_Renderer*);
int SDLEvents(int, int*, int, int, MapDiv**, Frota**, SDL_Renderer*, char*, float);
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window* );
int InitEverything(int , int , SDL_Window** , SDL_Renderer** );
int InitSDL();
int InitFont();
void RenderMap(int , int , Frota*, SDL_Renderer*, MapDiv**);
int RenderText(int , int , const char* , TTF_Font *, SDL_Renderer* );
int RenderLogo(int , int , SDL_Renderer*);

#endif // SDL_H_
