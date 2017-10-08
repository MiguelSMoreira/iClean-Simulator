/*************************************************************************************************************
 * Intituto Superior Tecnico                                                                                 *
 * Trabalho realizado por Miguel Moreira e Pedro Coimbra, alunos numeros 81048 e 81293 no ambito do curso de *
 * Mestrado em Engenharia Electrotecnica e Computadores.                                                     *
 *                                                                                                           *
 * Funcoes MovROB - Entrega Final                                                                            *
 * Funcoes responsaveis pelos movimentos dos robots durante a simulacao                                      *                                                                             *
 *                                                                                                           *
 * 22 de Abril de 2015                                                                                       *
 *************************************************************************************************************/

#include "MovROB.h"

/**
 * VerifCLEAN function: funcao responsavel por verificar se todos os quadrados da divisao foram
 * devidamente limpos
 */
int VerifCLEAN(MapDiv** array, int x, int y){

    int i=0, j=0, e=0;

    for(i = 0; i < y; i++)
        {
            for(j = 0; j < x; j++)
            {
                if ( array[i][j].est == 1)
                    e++;
            }
        }
    return e;
}

/**
 * CalcPos function: funcao responsavel por calcular a posicao seguinte de forma a alcancar
 * a posicao objectivo. Recebe ponteiro para robot a mover e altera por referencia o valor da
 * posicao deste
 */
void CalcPos(Frota* pont)
{
	int xc, yc, yt, xt;
	double angle;

	// just copy the values
	xc = pont->x;
	yc = pont->y;
	yt = pont->y_seg;
	xt = pont->x_seg;

	// calculate the angle
	angle = atan2((double)(yc-yt), (double)(xt-xc));

	// calculate the new position
	pont->x = floor(xc + cos(angle)+0.5);
	pont->y = floor(yc - sin(angle)+0.5);
	return;
}

/**
 * RandomFloat function: funcao que gera um numero (float) aleatorio entre "a" e "b"
 */
float RandomFloat(float a, float b){

    float random = ((float) rand() / (float) RAND_MAX);
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

/**
 * MovROB function: funcao responsavel pelo movimento do robot
 */
void MovROB(Frota* pont, MapDiv** array ,int x, int y, int delay, int a, float batmax)
{
    float bat;
    int X_aux, Y_aux;

    X_aux = pont->x;
    Y_aux = pont->y;
    strcpy(array[pont->y][pont->x].name , "NE");
    array[pont->y][pont->x].ocupied=0;

    CalcPos(pont);
    if (array[pont->y][pont->x].est == 2 || array[pont->y][pont->x].ocupied == 1 ){
        array[pont->y_seg][pont->x_seg].dest=0;
        pont->x = X_aux ;
        pont->y = Y_aux ;
        do {
            pont->x_seg = rand() % x;
            pont->y_seg = rand() % y;
            }while(pont->x == pont->x_seg || pont->y == pont->y_seg ||  pont->mapa[pont->y_seg][pont->x_seg].est != 0);
        }

    else {
        if (array[pont->y][pont->x].est == 0){
            pont->qd_limpos = (pont->qd_limpos) + 1;
        }
        pont->qd_perc = (pont->qd_perc) + 1;
    }
    array[pont->y_seg][pont->x_seg].dest=1;
    array[pont->y][pont->x].est=1;
    array[pont->y][pont->x].ocupied=1;
    pont->mapa[pont->y][pont->x].est=1;
    strcpy(array[pont->y][pont->x].name , pont->name);

    // Tratamento de Bateria
    bat =  RandomFloat(0.1 , batmax);
    bat = (pont->bat)- bat;
    pont-> bat=bat;

    return;
}
