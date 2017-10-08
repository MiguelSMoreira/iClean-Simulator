/*************************************************************************************************************
 * Intituto Superior Tecnico                                                                                 *
 * Trabalho realizado por Miguel Moreira e Pedro Coimbra, alunos numeros 81048 e 81293 no ambito do curso de *
 * Mestrado em Engenharia Electrotecnica e Computadores.                                                     *
 *                                                                                                           *
 * Funcoes SDL - Entrega Final                                                                               *
 * Conjunto de funcoes SDL necessarias a representacao grafica da simulacao da frota de robots iClean        *
 *                                                                                                           *
 * 22 de Abril de 2015                                                                                       *
 *************************************************************************************************************/

#include "SDL.h"

// Definicao de constantes (utilizadas para identificar a janela grafica)
const char myName[] = "   Miguel Moreira";   // Nome do Aluno
const char myNumber[] = "    81048 / 81293";       // Numero do Aluno
const char myName2[] = "   Pedro  Coimbra";   // Nome do Aluno

/**
 * Draw Line function: funcao responsavel por desenhar a linha que representa a direcao de movimento
 * necessaria ao robot seguir de forma a atingir a posicao objectivo seguinte
 */
 void DrawLine(Frota* pont, SDL_Renderer* _renderer){

    Frota* aux = pont;

    for(aux = pont; aux != NULL; aux = aux->seg){
        SDL_RenderDrawLine(_renderer, ((aux->x)*SQUARE_SIZE+(SQUARE_SIZE/2)), ((aux->y)*SQUARE_SIZE+(SQUARE_SIZE/2)), ((aux->x_seg)*SQUARE_SIZE+(SQUARE_SIZE/2)), ((aux->y_seg)*SQUARE_SIZE+(SQUARE_SIZE/2)));
    }
}

/**
 * SDLEvents function: funcao responsavel por lidar com os eventos de pressao de teclas.
 * Devolve o valor do delay actualizado pelo incremento e decremento com as setas e implementa a pausa
 * da simulacao, reinicio da simulacao, adicionar robots, sair da aplicacao e escrever o ficheiro
 * das estatisticas
 */
 int SDLEvents(int delay, int* ex, int x, int y, MapDiv** array, Frota** pont, SDL_Renderer* renderer, char* filename, float batmax)
 {
     int aux = 0, add = 0;
     float bat;
     int wr =0;
     int Posx, Posy;
     char rob[10], name;
     SDL_Event event;
     int est=0;

     if (*pont == NULL)
        aux = 1;

     while( SDL_PollEvent( &event ) )
    {
            // Caso o utilizador feche a janela grafica, o programa saira do loop while, terminando corretamente
            // o motor de Render e gerando o ficheiro das estatisticas do funcionamento ate esse momento.
			if( event.type == SDL_QUIT ){
                *ex=0;
                // Apagar lista e limpar mapa
                EraseMAPS(array, y);
                EraseLIST(*pont);
                printf("Simulacao terminada por ordem do utilizador.\n");
			}
			else if ( event.type == SDL_KEYDOWN )
			{
			    // Switch que permite utilizar as setas UP e DOWN de forma a controlar o delay e, consequentemente,
			    // a velocidade da simulacao
				switch ( event.key.keysym.sym )
				{
					case SDLK_DOWN:
                        delay= delay + 50;
						break;
					case SDLK_UP:
                        delay=delay-50;
                        // Condicao if que garante que o delay nunca e inferior a 50, de forma a prevenir o
                        // incorrecto funcionamento do programa
                        if (delay<=50)
                            delay=50;
                        break;
                    case SDLK_q:
                        *ex = 0;
                        EraseMAPS(array, y);
                        EraseLIST(*pont);
                        printf("Simulacao terminada por ordem do utilizador.\n");
                        break;
                    case SDLK_p:
                        aux = 1;
                        break;
                    case SDLK_e:
                        printf("Escrita Ficheiro.\n");
                        wr = WriteFILE(array, *pont);
                        if(wr == 0){
                            printf("Erro na Escrita do Ficheiro.");
                        }
                        break;
                    case SDLK_i:
                        RemROBOT(*pont, array);
                        EraseMAPS(array, y);
                        ReadFILE(filename);
                        RenderMap(x, y, *pont, renderer, array);
                        break;
					default:
						break;
				}
			 }
          }

    while ( aux == 1 )
    {
        while( SDL_PollEvent( &event ))
        {
            if( event.type == SDL_QUIT)
            {
                aux = 0;
                *ex=0;
                EraseMAPS(array, y);
                EraseLIST(*pont);
                printf("Simulacao terminada por ordem do utilizador.\n");
                // Apagar lista e limpar mapa
            }
            else if( event.type == SDL_KEYDOWN)
            {
                while(add == 1)
                {
                    if (event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z'){
                        name = (char)event.key.keysym.sym;
                        printf("R%c\n", name);
                        printf("Por favor selecione com o cursor uma posicao do mapa para colocar o robot.\n");
                        add = 2;
                    }
                    else {
                        printf("Por favor introduza uma letra valida.\nPressione a novamente para introduzir um robot.\n\n");
                        add = 0;
                    }
                }

                switch (event.key.keysym.sym)
                {
                    case SDLK_a:
                        if (add == 0)
                        {
                            add = 1;
                            printf("Introduza uma letra para designar o robot:\n");
                        }
                        break;

                    case SDLK_q:
                        if (add == 0)
                        {
                            EraseMAPS(array, y);
                            EraseLIST(*pont);
                            aux = 0;
                            *ex=0;
                        }
                        break;

                    case SDLK_p:
                        if (add == 0)
                        {
                            aux = 0;
                        }
                        break;

                    case SDLK_i:
                        if (add == 0)
                        {
                            RemROBOT(*pont, array);
                            EraseMAPS(array, y);
                            ReadFILE(filename);
                            RenderMap(x, y, *pont, renderer, array);
                        }
                        break;

                    case SDLK_e:
                        if (add == 0)
                        {
                            printf("Escrita Ficheiro.\n");
                            wr = WriteFILE(array, *pont);
                            if(wr == 0){
                                printf("Erro na Escrita do Ficheiro.");
                            }
                        }
                        break;

                    default:
                        break;

                }
            }
        }

        if(event.type == SDL_MOUSEBUTTONDOWN){
            SDL_GetMouseState(&Posx,&Posy);
            Posx = Posx/SQUARE_SIZE;
            Posy = Posy/SQUARE_SIZE;

            if(add == 2)
            {
                printf("Posicao selecionada: %d %d\n", Posx, Posy);
                if(Posx<x && Posy<y && array[Posy][Posx].est != 2)
                {
                    sprintf(rob, "R%c", name);
                    bat = RandomFloat(50.0 , 100.0);
                    *pont = AddROBOT(*pont, Posx, Posy, x, y, rob, array, bat, &est);
                    if (est==0){
                        array[Posy][Posx].ocupied = 1;
                        strcpy(array[Posy][Posx].name, rob);
                        array[Posy][Posx].est = 1;
                    }
                }
                else{
                    printf("Posicao ocupada com obstaculo.\n");
                }
                add = 0;
                RenderMap(x, y, *pont, renderer, array);
            }
        }
    }

    return delay;
 }

/**
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Renderer* _renderer)
{
	SDL_Texture *text_IST;
	SDL_Surface *img_IST;
	SDL_Rect boardPos;

	// renders IST Logo
	img_IST = SDL_LoadBMP("ist_logo.bmp");
	if (img_IST == NULL)
	{
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		exit(-5);
	}
	// square where the Logo is placed
	boardPos.x = x + 3;
	boardPos.y = y;
	boardPos.w = img_IST->w;
	boardPos.h = img_IST->h;

	// creates a texture and renders it in the screen
	text_IST = SDL_CreateTextureFromSurface(_renderer, img_IST);
	SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

	// destroy texture and surface
	SDL_DestroyTexture(text_IST);
	SDL_FreeSurface(img_IST);
	return img_IST->h;
}

/**
 * RenderText function: Renders the IST Logo on the window screen
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *font, SDL_Renderer* _renderer)
{
	SDL_Color color = { 0, 0, 0 };
	SDL_Surface *text_surface;
	SDL_Texture *text_texture;
	SDL_Rect solidRect;

	solidRect.x = x;
	solidRect.y = y;

	// creates a surface with some text
	text_surface = TTF_RenderText_Blended(font,text,color);
	if(!text_surface)
	{
	    printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
	    exit(-5);
	}

	// creates a texture from the surface and renders it !
	text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
	SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
	SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

	// destroy texture and surface
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
	return solidRect.h;
}

/**
 * RenderMap function: funcao modificada de forma a lidar com o render dos nomes e niveis de baterias dos
 * robots lateralmente e do render dos varios robots na simulacao
 *
 * Renders the map on the window screen according to their size
 * \param nSquareW number of squares to render (width)
 * \param nSquareH number of squares to render (height)
 * \param pos_robot position of the robot (ID of the square)
 * \param robot_name name of the robot (max. 2 letters)
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderMap(int nSquareW, int nSquareH, Frota* pont, SDL_Renderer* _renderer, MapDiv** array)
{
	TTF_Font *sans;
	TTF_Font *serif;
	SDL_Rect gridPos;
	char robot_name[10];
	int i, j, height, id = 0;
    Frota* aux;
    char buf[20];

	// set color of renderer to some color
	SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255);

	// clear the window
	SDL_RenderClear( _renderer );

	// opens a font style and sets a size
	  // Diminui o tamanho da fonte para o nome do Robot de forma a este caber, mesmo quando em maiusculas,
	  // nas dimensoes escolhidas para os quadrados da grelha
	sans = TTF_OpenFont("FreeSans.ttf", 17);
	serif = TTF_OpenFont("FreeSerif.ttf", 15);
	if(!sans || !serif)
	{
	    printf("TTF_OpenFont: %s\n", TTF_GetError());
	    exit(-5);
	}
	// render the IST Logo
	height = RenderLogo(nSquareW*SQUARE_SIZE, 0, _renderer);
	// render the student name
	height += RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height, myName, serif, _renderer);
	// render the student name
	height += RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height, myName2, serif, _renderer);
	// render the student name
    height += RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height, myNumber, serif, _renderer);

    aux=pont;
	while(aux != NULL)
    {
        sprintf(buf,"          %s %.0f", aux->name, aux->bat);
        // RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height, buf, serif, _renderer);
        height += RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height, buf, serif, _renderer);
        aux=aux->seg;
    }
	// grid position
	gridPos.w = SQUARE_SIZE;
	gridPos.h = SQUARE_SIZE;
	gridPos.y = 0;

	// iterate over all squares
	for (i = 0; i < nSquareH; i++)
	{
		gridPos.x = 0;
		for (j = 0; j < nSquareW; j++)
		{
		    // Condicao utilizada para garantir que nao e pintado de laranja o espaco correspondente a
		    // um quadrado ja limpo. Desta forma os quadrados ja limpos iram aparecer como brancos.
		    // Para tal inclui uma condicao if que apenas permite o Render da Cor apos verificar que a posicao
		    // que esta a ser pintada assume valor zero no vetor M (que guarda as posicoes por limpar com o
            // valor zero). Para tal tambem inclui o vetor M como argumento desta funcao.

            if (array[i][j].est==2)
            {
                SDL_SetRenderDrawColor( _renderer, 255, 0, 0, 0 );
                SDL_RenderFillRect( _renderer, &gridPos );
            }

            else if (array[i][j].est==0)
            {  // writes a dirty square
                SDL_SetRenderDrawColor( _renderer, 221, 161, 135, 255 );
                SDL_RenderFillRect( _renderer, &gridPos );
            }

            if (array[i][j].dest==1)
            {  // writes a dirty square
                SDL_SetRenderDrawColor( _renderer, 127, 255, 0, 0 );
                SDL_RenderFillRect( _renderer, &gridPos );
            }

            SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
			SDL_RenderDrawRect( _renderer, &gridPos );


            if (strcmp(array[i][j].name, "NE") !=0)
            {
                strcpy(robot_name, array[i][j].name);
                RenderText(gridPos.x+MARGIN, gridPos.y+2, robot_name, sans, _renderer);
            }

			gridPos.x += SQUARE_SIZE;
			id++;
		}
		gridPos.y += SQUARE_SIZE;
	}


	DrawLine(pont, _renderer);
	// render the changes above
	SDL_RenderPresent( _renderer);

	// destroy everything
	TTF_CloseFont(sans);
	TTF_CloseFont(serif);
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
int InitEverything(int width, int height, SDL_Window** _window, SDL_Renderer** _renderer)
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	if ( !InitSDL() )
		return 0;

	if ( !InitFont() )
		return 0;

	window = CreateWindow(width, height);
	if ( window == NULL )
		return 0;

	renderer = CreateRenderer(width, height, window);
	if ( renderer == NULL )
		return 0;

	*_window = window;
	*_renderer = renderer;

	return 1;
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
int InitSDL()
{
	// init SDL library
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		printf(" Failed to initialize SDL : %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
int InitFont()
{
	// init font library
	if(TTF_Init()==-1)
	{
	    printf("TTF_Init: %s\n", TTF_GetError());
	    return 0;
	}

	return 1;
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
	SDL_Window* window = NULL;
	// init window
	window = SDL_CreateWindow( "iClean", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );

	if ( window == NULL )
	{
		printf("Failed to create window : %s\n", SDL_GetError());
		return NULL;
	}

	return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window* _window)
{
	SDL_Renderer* renderer;
	// init renderer
	renderer = SDL_CreateRenderer( _window, -1, 0 );

	if ( renderer == NULL )
	{
		printf("Failed to create renderer : %s", SDL_GetError());
		return NULL;
	}
	// set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

	return renderer;
}
