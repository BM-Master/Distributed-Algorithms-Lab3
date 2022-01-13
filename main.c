#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/* 
    TODO:
        1- Desde el proceso master se debe leer el archivo por medio de una función.
        2- Crear otra función en donde se enviará la información a cada proceso, esta
            información consiste en la porción del tablero que va a procesar.
        3- Ya luego definir la función que procesará la información y arrojará el resultado
            de si un área pertenece a un color de ficha o no.
*/

int borde_esquina(int i, int j, int** tablero, int dimension);
unsigned char **readFile(int dim);
void imprimeTablero(unsigned char **tablero, int dim);

MPI_Status status;

#define MASTER 0
#define TAG 1 // traspaso de resultados {0, 1} con 0 que ganó el área la ficha negra y 1 que ganó la ficha blanca
#define TAG_DIMENSION 2 // traspaso de la dimensión del tablero de cada proceso


int main(int argc, char *argv[])
{
    int dimension;
    unsigned char **tablero;
    unsigned int negras, blancas; // negras -> 1 blancas -> 2
    unsigned int temp1, temp2;

    unsigned int whoami, hmaw, me;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    // scanf("%i", &dimension);

    // tablero = (int**) calloc(dimension,sizeof(int*)); // Variable global?
    // for (int i = 0; i < dimension; i++)
    // {
    //    tablero[i] = (int*) calloc(dimension,sizeof(int));
    // }
    // // Tablero inicializado en cero por calloc.


    // // Número 1 para las piezas negras.
    // scanf("%i",&negras);
    // for(int j=0; j<negras; j++)
    // {
    //     scanf("%i %i", &temp1, &temp2);
    //     tablero[temp1][temp2]=1;
    // }

    // // Número 2 para las piezas blancas.
    // scanf("%i",&blancas);
    // for(int j=0; j<negras; j++)
    // {
    //     scanf("%i %i", &temp1, &temp2);
    //     tablero[temp1][temp2]=2;
    // }

    //------Inicio de funciones------//

    /* MPI things */
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD,&hmaw); 
    MPI_Comm_rank(MPI_COMM_WORLD,&whoami);
    MPI_Get_processor_name(processor_name,&me); 
    printf("Process [%d] Alive on %s\n",whoami,processor_name); 
    fflush(stdout);

    
    // if(whoami == MASTER){

    // }

    // TODO LO QUE PASE AQUÍ ES PARALELO 

    MPI_Barrier(MPI_COMM_WORLD);

    // ejecutar función para cada proceso 

    MPI_Finalize();



    return 0;
}


/*
    Uso: Lee un archivo de texto con la información del tablero y devuelve una matriz
         con la información.
    Entrada: dimensión del tablero
    Salida: tablero con las posiciones de las fichas negras y blancas
*/

unsigned char **readFile(int dim){

    unsigned char **tablero;
    int i,j;

    tablero = calloc(dim + 2,sizeof(unsigned char *)); // dim + 2 para no preocuparse de los bordes
    for(i = 0; i < dim + 2; i++){
        tablero[i] = calloc(dim + 2,sizeof(unsigned char));
    }

    for(i = 1 ; i <= dim; i++){
        for(j = 1; j <= dim; j++){
            scanf("%d", &tablero[i][j]);
        }
    }

    return tablero;
}

/*
    Uso: Imprime el tablero dado en consola
    Entrada: tablero y su dimensión
    Salida: Solo un print del tablero
*/

void imprimeTablero(unsigned char **tablero, int dim){
    int i,j;
    for(i = 0; i <= dim + 1; i++){
        for(j = 0; j <= dim + 1; j++){
            printf("%d ", tablero[i][j]);
        }
        printf("\n");
    }
}

int borde_esquina(int i, int j, int** tablero, int dimension)
{
    // i: filas, j: columnas

    if(i == 0 && j == 0)
    {
        return 1; //Esquina superior izquierda
    }

    if(i == 0 && j == dimension-1)
    {
        return 2; //Esquina superior derecha
    }
    
    if(i == dimension-1 && j == dimension-1)
    {
        return 3; //Esquina inferiror derecha
    }

    if(i == dimension-1 && j == 0)
    {
        return 4; //Esquina inferior izquierda
    }

    if(i == 0 && 0 < j < dimension-1 )
    {
        return 5; //Borde izquierdo
    }

    if(i == 0 && 0 < j < dimension-1)
    {
        return 6; //Borde superiror
    }

    if(i == dimension-1  && 0 < i < dimension-1)
    {
        return 7; //Borde derecho
    }

    if(0 < i < dimension-1 && j == dimension-1)
    {
        return 8; //Borde inferior
    }
    else
    {
        return 0; //Piezas sin tope en sus vecinos.
    }
}