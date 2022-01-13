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

MPI_Status status;
unsigned int whoami, hmaw;

#define MASTER 0
#define TAG 1 // traspaso de resultados {0, 1} con 0 que ganó el área la ficha negra y 1 que ganó la ficha blanca
#define TAG_DIMENSION 2 // traspaso de la dimensión del tablero de cada proceso


int main(int argc, char *argv[])
{
    int dimension, **tablero;
    unsigned int negras, blancas;
    unsigned int temp1, temp2;

    scanf("%i", &dimension);

    tablero = (int**) calloc(dimension,sizeof(int*)); // Variable global?
    for (int i = 0; i < dimension; i++)
    {
       tablero[i] = (int*) calloc(dimension,sizeof(int));
    }
    // Tablero inicializado en cero por calloc.


    // Número 1 para las piezas negras.
    scanf("%i",&negras);
    for(int j=0; j<negras; j++)
    {
        scanf("%i %i", &temp1, &temp2);
        tablero[temp1][temp2]=1;
    }

    // Número 2 para las piezas blancas.
    scanf("%i",&blancas);
    for(int j=0; j<negras; j++)
    {
        scanf("%i %i", &temp1, &temp2);
        tablero[temp1][temp2]=2;
    }

    //------Inicio de funciones------//

    /* MPI things */
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD,&hmaw); 
    MPI_Comm_rank(MPI_COMM_WORLD,&whoami);
    MPI_Get_processor_name(processor_name,&me); 
    printf("Process [%d] Alive on %s\n",whoami,processor_name); 
    fflush(stdout);

    // TODO LO QUE PASE AQUÍ ES PARALELO 

    MPI_Barrier(MPI_COMM_WORLD);

    // ejecutar función para cada proceso 

    MPI_Finalize();



    return 0;
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