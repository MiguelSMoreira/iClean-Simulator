/*************************************************************************************************************
 * Intituto Superior Tecnico                                                                                 *
 * Trabalho realizado por Miguel Moreira e Pedro Coimbra, alunos numeros 81048 e 81293 no ambito do curso de *
 * Mestrado em Engenharia Electrotecnica e Computadores.                                                     *
 *                                                                                                           *
 * Funcoes ListMANIP - Entrega Final                                                                         *
 * Conjunto de funcoes responsaveis pela criacao e manipulacao de listas                                     *
 *                                                                                                           *
 * 22 de Abril de 2015                                                                                       *
 *************************************************************************************************************/

#include "ListMANIP.h"

/**
 * FillLIST function: Recebe ponteiro para lista de robot a preencher e a posicao em que se encontra (x,y). Preenche
 * a lista (excepto o ponteiro seg e ant ja preenchidos no Add Robot) com o nome do Robot, as suas
 * coordenadas, gera as coordenadas seguintes iniciais, gera um nivel de bateria aleatorio entre 50 e 100
 * e associa-lhe e preenche o mapa da divisao do robot
 */
void FillLIST(Frota* pont, int x_rob, int y_rob, int x, int y, char* rob, MapDiv** array, float bat){

    strcpy(pont->name, rob);
    pont->x = x_rob;
    pont->y = y_rob;
    pont->bat= bat;
    pont->mapa = DynPointerARRAY(x,y);
    pont->qd_limpos = 0;
    pont->qd_perc = 0;

    do {
        pont->x_seg = rand() % x;
        pont->y_seg = rand() % y;
        }while(pont->x == pont->x_seg || pont->y == pont->y_seg ||  pont->mapa[pont->y_seg][pont->x_seg].est != 0);

        array[pont->y_seg][pont->x_seg].dest=1;

    return;
}

/**
 * AddROBOT function: Recebe ponteiro para inicio da Lista, aloca memoria para robot e organiza-o na lista
 * e chama funcao para preencher a estrutura criada.
 * Retorna ponteiro para novo inicio da lista.
 */
Frota* AddROBOT(Frota* pont, int x_rob , int y_rob, int x, int y, char* rob, MapDiv** array, float bat, int* est)
{
    Frota* novo = NULL;
    Frota* aux2 = NULL;
    Frota* aux4 = NULL;
    int diff;

    novo = (Frota*) malloc(sizeof (Frota));
    FillLIST(novo, x_rob, y_rob, x, y, rob, array, bat);

    if(pont == NULL){
        novo->seg = NULL;
        novo->ant = NULL;
        pont=novo;
    }

    else if (pont->ant != NULL){
        printf("O apontador nao aponta para o inicio da lista.\n ");
        free(novo);
    }

    else if (pont->ant == NULL){
        aux2 = pont;
        if (aux2->seg==NULL){   // A lista so tem uma estrutura
            diff = strcmp(rob, aux2->name);
            if(diff > 0){
                aux2->seg = novo;
                novo->seg = NULL;
                novo->ant = aux2;
            }
            if(diff < 0){
                novo->seg = aux2;
                novo->ant = NULL;
                aux2->ant=novo;
                pont=novo;
            }
            if(diff == 0){
                printf("Um robot com esse nome ja se encontra no mapa.\n");
                *est=1;
                free(novo);
            }
        }
        else{ // A lista tem mais que um elemento
            do{
                diff = strcmp(rob, aux2->name);
                if (diff == 0){
                    printf("Um robot com esse nome ja se encontra no mapa.\n");
                    *est=1;
                    free(novo);
                }

                if (diff < 0){
                    if(aux2->ant == NULL){
                        aux2->ant = novo;
                        novo->seg = aux2;
                        novo->ant = NULL;
                        pont=novo;
                    }
                    else {
                        novo->seg = aux2;
                        novo->ant = aux4;
                        aux2->ant = novo;
                        aux4->seg = novo;
                    }
                }

                if (diff > 0){
                    aux4 = aux2;
                    aux2 = aux2->seg;
                }

                if (aux2 == NULL){
                    aux4->seg = novo;
                    novo->ant = aux4;
                    novo->seg = NULL;
                    diff = 0;
                }

            }while( diff > 0);

        }
    }
    return pont;
}

/**
 * RemROBOT function: Recebe ponteiro para robot a remover, dealoca memoria e devolve um ponteiro para o
 * novo inicio da lista
 */
Frota* RemROBOT(Frota* pont, MapDiv** array)
{
    Frota* aux = NULL;
    Frota* aux2 = NULL;
    Frota* aux3 = NULL;


    if (pont->ant == NULL && pont->seg == NULL){
        strcpy(array[pont->y][pont->x].name, "NE");
        array[pont->y_seg][pont->x_seg].dest = 0;
        free(pont);
        return NULL;
    }

    else if (pont->ant == NULL){
        aux=pont;
        pont=pont->seg;
        pont->ant=NULL;
        strcpy(array[aux->y][aux->x].name, "NE");
        array[aux->y_seg][aux->x_seg].dest = 0;
        free(aux);
    }

    else if (pont->seg == NULL){
        aux = pont;
        pont=pont->ant;
        pont->seg=NULL;
        strcpy(array[aux->y][aux->x].name, "NE");
        array[aux->y_seg][aux->x_seg].dest = 0;
        free(aux);
        while(pont->ant != NULL){
            pont=pont->ant;
        }
    }

    else{
        aux3=pont;
        aux=pont->seg;
        aux2=pont->ant;
        aux->ant=aux2;
        aux2->seg=aux;
        while(pont->ant != NULL){
            pont=pont->ant;
        }
        strcpy(array[aux3->y][aux3->x].name, "NE");
        array[aux3->y_seg][aux3->x_seg].dest = 0;
        free(aux3);
    }
    return pont;
}

/**
 * EraseLIST function: funcao responsavel por limpar toda a memoria alocada para a lista dos robots
 */
void EraseLIST(Frota* pont){

    Frota* aux;
    aux=pont;
    while (pont != NULL){
        aux = aux->seg;
        free(pont);
        pont = aux;
    }

    return;
}

/**
 * EraseMAPS function: funcao responsavel por limpar toda a memoria alocada para a matriz bi-dimensional
 * de estruturas que representa o mapa da divisao
 */
void EraseMAPS(MapDiv** array, int y){
    free(array);
    return;
}
