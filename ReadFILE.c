/*************************************************************************************************************
 * Intituto Superior Tecnico                                                                                 *
 * Trabalho realizado por Miguel Moreira e Pedro Coimbra, alunos numeros 81048 e 81293 no ambito do curso de *
 * Mestrado em Engenharia Electrotecnica e Computadores.                                                     *
 *                                                                                                           *
 * Funcoes ReadFILE - Entrega Final                                                                          *
 * Funcoes responsaveis pela leitura do ficheiro com as informacoes do mapa da divisao, pela escrita do      *
 * ficheiro com as estatisticas da simulacao (estatisticas.txt) e pelo alocamento dinamico das listas e      *
 * matrizes de estruturas necessarias ao projecto                                                            *                                                            *
 *                                                                                                           *
 * 22 de Abril de 2015                                                                                       *
 *************************************************************************************************************/

#include "ReadFILE.h"

/**
 * WriteFILE function: funcao responsavel por escrever o ficheiro com as estatisticas da simulacao
 */
int WriteFILE(MapDiv** array, Frota* pont){

    FILE* fw;
    Frota* aux;

    aux=pont;
    // Conjunto de funcoes utilizadas para a escrita do ficheiro estatisticas.txt
    // Abertura do ficheiro em modo de escrita
    fw=fopen("estatísticas.txt","w");
    // Condicao que garante robustez do programa para o caso de nao ser possivel abrir/criar o ficheiro
    // estatisticas.txt
    if (fw==NULL)
    {
        printf("Nao foi possivel a criacao do ficheiro com as estatisticas.");
        return(0);
    }

    while (aux != NULL){
    // Escrita no ficheiro das variaveis Nome do Robot, Numero de Quadrados Limpos e do Numero de Quadrados Percorridos
    fprintf(fw,"Nome do Robot: %s\n\t Quadrados Limpos: %d\n\t Quadrados Percorridos: %d\n\n", aux->name, aux->qd_limpos, aux->qd_perc);
    aux=aux->seg;
    }

    fclose(fw);

    return(1);
}

/**
 * ListALLOC function: funcao responsavel pela criacao da lista inicial (aquando da leitura do ficheiro
 * inicial)
 */
Frota* ListALLOC(int a){

    Frota *pont = NULL;
    Frota *aux3 = NULL;
    Frota *aux4 = NULL;
    int k=0, z;

    for(z=0; z<a; z++)
    {
        aux3 = (Frota*) malloc(sizeof (Frota));
        if(aux3==NULL)
        {
            printf("Erro na Alocacao de Memoria");
            exit(EXIT_FAILURE);
        }
        aux3->seg = NULL;

        if(k==0)
        {
            pont=aux3;
            pont->ant = NULL;
            k=1;
        }
        else
        {
            aux3->ant = aux4;
            aux4->seg = aux3;
        }
        aux4=aux3;
    }
    return pont;
}

/**
 * DynPointerARRAY function: funcao responsavel por criar a matriz bidimensional de estruturas por
 * alocacao dinamica, que compoe o mapa da divisao
 */
MapDiv** DynPointerARRAY(int x, int y){

    int i=0, j=0;
    MapDiv** array;
    array = malloc(y* sizeof(y));
    if(array==NULL)
    {
        fprintf(stderr, "Out of Memory.\n");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < y; i++)
    {
        array[i] = malloc(x* sizeof(MapDiv));
        if(array[i] == NULL)
        {
            fprintf(stderr, "Out of Memory.\n");
            exit(EXIT_FAILURE);
        }
    }

     // Modificar MALLOC para CALLOC na alocacao de cima
    for(i = 0; i < y; i++)
    {
        for(j = 0; j < x; j++)
        {
            array[i][j].est=0;
            array[i][j].ocupied=0;
            array[i][j].dest=0;
            strcpy(array[i][j].name , "NE");
        }
    }
    return array;
}

/**
 * ReadFILE function: funcao responsavel por "ler" as caracteristicas do ficheiro inicial. Esta chama
 * as funcoes para criacao da lista de robots e da matriz bidimensional de estruturas e preenche-as
 * com as caracteristicas lidas do ficheiro inicial. Retorna um ponteiro para uma estrutura de dados
 * (ReadPointers) criada de forma a permitir a passagem de varios parametros lidos para a funcao main
 */
Pointers* ReadFILE(char* filename){

    FILE *fp;
    int x, y, i , j, oc;
    char buf[20], rob[10];
    int aux, aux2, a=1, c;
    float bat;
    MapDiv** array = NULL;
    Frota* pont = NULL;
    Pointers* aux3 = NULL;
    int est=0;

    fp=fopen(filename ,"r");
    // Condicao que garante robustez do programa para o caso de nao ser possivel abrir/criar o ficheiro
    // estatisticas.txt
    if (fp==NULL)
    {
        printf("Nao foi possivel abrir o ficheiro de controlo.\n");
        exit(EXIT_FAILURE);
    }

    fgets(buf, sizeof buf, fp);

    // Adicionar condicao de ma leitura
    sscanf(buf, "%d %d %d", &x, &y, &oc);
    array = DynPointerARRAY(x, y);
    aux3 = malloc(sizeof (struct ReadPointers));
    aux3->mapa=array;
    aux3->x=x;
    aux3->y=y;
    aux3->oc=oc;

    do
    {
       fgets(buf, sizeof buf, fp);
       // Adicionar condicao de ma leitura
       if (sscanf(buf, "[%d, %d]", &aux, &aux2) !=2)
            a=0;
       else
       {
            array[aux2][aux].est=2;
       }
    } while(a);

    // Adicionar condicao de ma leitura
    sscanf(buf, "%d", &a);
    aux3->a=a;

    for(c=0; c<a; c++)
    {
       fgets(buf, sizeof buf, fp);
       // Adicionar condicao de ma leitura
       sscanf(buf, "%s [%d, %d] %f", rob, &aux, &aux2, &bat);
       pont = AddROBOT(pont, aux, aux2, x, y, rob, array, bat, &est);
       array[aux2][aux].est=1;
       array[aux2][aux].ocupied=1;
       pont->mapa[aux2][aux].est=1;
       strcpy(array[aux2][aux].name, rob);
    }
    aux3->frota = pont;

    // ALTERAR DE FORMA A NAO CARREGAR OS PONTOS COM 2
    ////////////////////////////////////////////////////////////////
    while(pont != NULL){
        for(i = 0; i< y; i++){
            for(j = 0; j< x; j++){
                pont->mapa[i][j].est = array[i][j].est;
           }
        }
        pont=pont->seg;
    }

    fclose(fp);

    return aux3;
}
