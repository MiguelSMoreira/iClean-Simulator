/*************************************************************************************************************
 * Intituto Superior Tecnico                                                                                 *
 * Trabalho realizado por Miguel Moreira e Pedro Coimbra, alunos numeros 81048 e 81293 no ambito do curso de *
 * Mestrado em Engenharia Electrotecnica e Computadores.                                                     *
 *                                                                                                           *
 * Projecto Robot de Limpeza iClean - Entrega Final                                                          *
 *                                                                                                           *
 * 22 de Abril de 2015                                                                                       *
 *                                                                                                           *
 * Descricao do Programa                                                                                     *
 * O programa simula o funcionamento de uma frota de robots de limpeza que se movimentam num piso limpando   *
 * a area por onde passam.                                                                                   *
 * Este permite assim a geracao do mapa da divisao a partir da leitura de um ficheiro com as dimensoes da    *
 * divisao, numero de quadrados ocupados, coordenadas destes e das informacoes relativas a frota de robots   *
 * inicial. Durante a simulacao e possivel gravar um ficheiro com as estatisticas de limpeza dos robots,     *
 * pausar a simulacao, adicionar robots a divisao (enquanto esta se encontre em pausa), reiniciar a          *
 * simulacao, aumentar e diminuir a velocidade da simulacao e sair do programa.                              *                                                               *
 *************************************************************************************************************/

// Inclusao das Bibliotecas necessarias ao projecto
#include "Structs.h"
#include "SDL.h"
#include "ReadFILE.h"
#include "MovROB.h"
#include "ListMANIP.h"

int main(int argc,char* argv[])
{
    // Definicao de Variaveis Locais
    char filename[10];
    int height=0, width=0, oc=0, e1=0, e2=0, a=0, ex=1, x=0, y=0, delay = 300;
	float bat=0;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	MapDiv** array = NULL;
	Pointers* aux = NULL;
    Frota* pont = NULL;
    Frota* ax = NULL;
    srand((unsigned) time(NULL));

    // Colocacao dos argumentos do programa (Argv e Argc) nas variaveis filename (nome do ficheiro mapa) e
    //bat (que representa o nivel de bateria maxima que pode ser retirado por movimento do robot)
    if (strcpy(filename, argv[1]) == NULL){
        printf("\nNome invalido.\nPor favor nao se esqueca de introduzir a extensao do ficheiro (.map)\n");
        return(EXIT_FAILURE);}

    if (sscanf(argv[2],"%f", &bat) != 1) {
        printf("\nErro a receber o valor maximo para a bateria.\n");
        return(EXIT_FAILURE);}

    if (bat<=0.1 || bat >100.0){
        printf("\nValor maximo para a bateria invalido.\nPor favor introduza um valor entre 0,1 e 100.\n");
        return(EXIT_FAILURE);}

    // Leitura do Ficheiro com o Mapa da Divisao e repetivo preenchimento das variaveis x, y (dimensoes da
    //divisao), oc (numero de posicoes com obstaculos), pont (ponteiro para a lista de robots) e array
    //(ponteiro para o mapa da divisao) da estrutura aux, criada para retirar estes parametros da funcao ReadFile
    aux=ReadFILE(filename);
    pont=aux->frota;
    array=aux->mapa;
    x=aux->x;
    y=aux->y;
    oc=aux->oc;

    // Calculo da altura e largura da janela, e chamada da funcao InitEverything reponsavel pela inicializacao
    //grafica. Calculo de e1 a que corresponde o numero de quadrados a limpar

	width = SQUARE_SIZE*x;
	height = SQUARE_SIZE*y;
    if ( !InitEverything(width, height, &window, &renderer) )
		return -1;

    e1 = (x*y)-oc;

    // Ciclo responsavel pelo movimento efectivo do Robot. O primeiro ciclo while garante que todas as
    //posicoes da lista de robots sao corridas . Dentro deste encontra-se uma condicao responsavel por
    //gerar as posicoes objectivas seguintes caso o robot atinja as anteriormente definidas.
    //Condicao MovROB responsavel pela atualizacao das coordenadas do robot (x,y) de forma a alcancar a
    //posicao alvo definida. Por fim a condicao if e responsavel por verificar se a bateria do robot
    //se esgotou devido ao ultimo movimento, nesse caso esse robot sera removido.
    while(ex){
        ax = pont;
        // Tratamento de Posicoes
        while(ax != NULL ){
            if(ax->x == ax->x_seg && ax->y == ax->y_seg){
                array[ax->y_seg][ax->x_seg].dest=0;
                do {
                    ax->x_seg = rand() % x;
                    ax->y_seg = rand() % y;
                    }while(ax->x == ax->x_seg || ax->y == ax->y_seg || (ax->mapa)[ax->y_seg][ax->x_seg].est != 0);
                array[ax->y_seg][ax->x_seg].dest=1;
            }
            MovROB(ax, array,x , y, delay, a, bat);
            if( ax->bat <= 0){
                array[ax->y_seg][ax->x_seg].dest=0;
                pont = RemROBOT(ax, array);
            }
            ax=ax->seg;
        }

        //Funcoes reponsaveis por fazer render do mapa de acordo com as novas condicoes (calculadas para cada
        //um dos robots no ciclo while anterior). Assim todas as modificacoes seram realizadas ao mesmo tempo
        //havendo um movimento continuo de todos os robots. A funcao SDLEvents trata de todos os eventos
        //posiveis durante a simulacao (pausa, reiniciar, etc) ao mesmo tempo que alimenta a funcao SDL_Delay
        //(responsavel por regular a velocidade da simulacao) com o delay apropriado.
        RenderMap(x, y, pont, renderer, array);
        delay = SDLEvents(delay, &ex, x, y, array, &pont, renderer, filename, bat);
        SDL_Delay(delay);

        //Condicao que verifica a saida do ciclo dos movimentos. Esta gera o numero de quadrados que ja foram
        //limpos durante a simulacao e compara-os com o numero total de quadrados que precisam de ser limpos
        //para a simulacao poder terminar. Assim e verificado se toda a divisao ja foi limpa (se tiver sido
        //ex=0 e o ciclo responsavel pelo movimento sera terminado)
        e2 = VerifCLEAN(array, x, y);
        if (e1 == e2){
            printf("Final da Simulacao.\n");
            ex=0;
        }
    }

    //Funcoes SDL responsaveis por terminar a janela grafica
 	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return EXIT_SUCCESS;
}
