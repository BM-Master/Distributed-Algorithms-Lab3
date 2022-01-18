#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
    TODO:
        1- Hacer el envío a cada proceso desde el Master con su porción de tablero correspondiente
        2- Ya luego definir la función que procesará la información y arrojará el resultado
            de si un área pertenece a un color de ficha o no.
*/

int borde_esquina(int i, int j, int **tablero, int dimension);
unsigned char **readFile(int dim);
unsigned char **leerTablero(int dim);
unsigned char **crearSubTableroVertical(unsigned char **tablero, int colInicio, int colFin, int filas);
void imprimeTablero(unsigned char **tablero, int filas, int columnas);
void imprimeTableroConCeros(unsigned char **tablero, int filas, int columnas);
void reparteTablero(unsigned char **tablero, int dim, int numProcesos);
void testRecover();

MPI_Status status;

#define MASTER 0
#define TAG 1           // traspaso de resultados {0, 1} con 0 que ganó el área la ficha negra y 1 que ganó la ficha blanca
#define TAG_DIMENSION 2 // traspaso de la dimensión del tablero de cada proceso

int dimension;
unsigned int whoami, hmaw;

int main(int argc, char *argv[])
{
    int dimension;
    unsigned char **tablero;
    unsigned int negras, blancas; // negras -> 1 blancas -> 2
    unsigned int temp1, temp2;

    unsigned int me;
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
    MPI_Comm_size(MPI_COMM_WORLD, &hmaw);
    MPI_Comm_rank(MPI_COMM_WORLD, &whoami);
    MPI_Get_processor_name(processor_name, &me);
    printf("Process [%d] Alive on %s\n", whoami, processor_name);
    fflush(stdout);

    if (whoami == MASTER)
    {
        scanf("%i", &dimension);
        tablero = readFile(dimension);
        imprimeTableroConCeros(tablero, dimension, dimension);

        reparteTablero(tablero, dimension, hmaw); // parece que esta función va en el testRecover dentro del master
    }

    MPI_Barrier(MPI_COMM_WORLD);

    testRecover();

    MPI_Finalize();

    return 0;
}

// TESTEO DE ENVÍO DE MENSAJES
void testRecover()
{

    unsigned char *c;
    c = calloc(3, sizeof(unsigned char));

    int dim2 = 8;

    int a;
    int a2;
    int a3;

    if (whoami == MASTER)
    {
        MPI_Send((void *)&dim2, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        printf("Master enviado dim2\n");
    }
    else
    {
        unsigned char **tablero;
        printf("PREVIO A RECIBIR! \n");
        MPI_Recv((void *)&a, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        printf("status tag: %d\n", status.MPI_TAG);
        printf("a = %d\n", a);

        MPI_Recv(c, 3, MPI_UNSIGNED_CHAR, 0, 1, MPI_COMM_WORLD, &status);
        printf("tt2: %d\n", c[1]); // recibo una fila

        MPI_Recv((void *)&a3, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        printf("status tag: %d\n", status.MPI_TAG);
        printf("a3 = %d\n", a3);
    }
}

/*
    Uso: Lee un archivo de texto con la información del tablero y devuelve una matriz
         con la información pero con ceros extras.
    Entrada: dimensión del tablero más dos
    Devuelve: tablero con las posiciones de las fichas negras y blancas
*/

unsigned char **readFile(int dim)
{

    unsigned char **tablero;
    int i, j;

    tablero = calloc(dim + 2, sizeof(unsigned char *)); // dim + 2 para no preocuparse de los bordes
    for (i = 0; i < dim + 2; i++)
    {
        tablero[i] = calloc(dim + 2, sizeof(unsigned char));
    }

    for (i = 1; i <= dim; i++)
    {
        for (j = 1; j <= dim; j++)
        {
            scanf("%d", &tablero[i][j]);
        }
    }

    return tablero;
}

// unsigned char **contiguo(int dim){
//     int i;

//     char *data = (int *)malloc(rows*cols*sizeof(int));
//     char **array= (int **)malloc(rows*sizeof(int*));
//     for (i=0; i<rows; i++)
//         array[i] = &(data[cols*i]);

//     return array;
// }

/*
    Uso: Lee un archivo de texto con la información del tablero y devuelve una matriz
         con la información pero sin ceros extras.
    Entrada: dimensión del tablero
    Devuelve: tablero con las posiciones de las fichas negras y blancas
*/

unsigned char **leerTablero(int dim)
{

    unsigned char **tablero;
    int i, j;

    tablero = calloc(dim, sizeof(unsigned char *)); // dim + 2 para no preocuparse de los bordes
    for (i = 0; i < dim; i++)
    {
        tablero[i] = calloc(dim, sizeof(unsigned char));
    }

    for (i = 0; i < dim; i++)
    {
        for (j = 0; j < dim; j++)
        {
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

void imprimeTablero(unsigned char **tablero, int filas, int columnas)
{
    int i, j;
    for (i = 1; i <= filas; i++)
    {
        for (j = 1; j <= columnas; j++)
        {
            printf("%d ", tablero[i][j]);
        }
        printf("\n");
    }
}

/*
    Uso: Imprime el tablero con ceros dado en consola
    Entrada: tablero y su dimensión
    Salida: Solo un print del tablero
*/

void imprimeTableroConCeros(unsigned char **tablero, int filas, int columnas)
{
    int i, j;

    for (i = 0; i <= filas + 1; i++)
    {
        for (j = 0; j <= columnas + 1; j++)
        {
            printf("%d ", tablero[i][j]);
        }
        printf("\n");
    }
}

/*
    Uso: Reparte el tablero entre los K procesos
    Entrada: tablero, dimensión y el número de procesos
    Salida: tablero repartido
*/

void reparteTablero(unsigned char **tablero, int dim, int nprocesos)
{
    printf("dimensiones y procesos: %d %d\n", dim, nprocesos);
    int dimPorProceso = dim / nprocesos; // columnas que utilizará cada proceso
    int i, j, c = 0;

    unsigned char **testSubTablero;

    printf("%d\n", dim % nprocesos);

    if (dim % nprocesos != 0)
    { // si es que no es divisible
        int extra = (dim % nprocesos);
        int dimPorProcesoExtra = dimPorProceso + extra;
        printf("Un proceso tendrá más trabajo %d\n", dimPorProcesoExtra);
    }

    printf("Dimensiones por proceso %d\n", dimPorProceso);
    testSubTablero = crearSubTableroVertical(tablero, 0, 1, dim);

    dimension = dim + 2;
    int dim2 = 5;
    int dimt = 32;

    MPI_Send((void *)&dim2, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);               // enviando al proceso 1 la dimensión del tablero
    MPI_Send(testSubTablero[1], 3, MPI_UNSIGNED_CHAR, 1, 1, MPI_COMM_WORLD); // enviando al proceso 1 la fila 1
}

/*
    Uso: Crea un subtablero para cada proceso
    Entrada: tablero, filas y columnas del subtablero
    Salida: No hay
    Nota: Partición vertical
*/
unsigned char **crearSubTableroVertical(unsigned char **tablero, int colInicio, int colFin, int filas)
{
    unsigned char **subTablero;
    unsigned char **P;
    int i, j;
    int k = colFin - colInicio;

    subTablero = calloc(filas + 2, sizeof(unsigned char *)); // filas
    for (i = 0; i < filas + 2; i++)
    {
        subTablero[i] = calloc(k + 2, sizeof(unsigned char)); // columnas
    }

    // P = subTablero;
    // imprimeTableroConCeros(P, filas, k);

    for (i = 1; i <= filas; i++)
    {
        for (j = 1; j <= colFin; j++)
        {
            subTablero[i][j] = tablero[i][colInicio + j];
        }
    }

    imprimeTableroConCeros(subTablero, filas, k);

    return subTablero;
}


// Solo para implementar la lógica de lo que contienen las piezas

// dimensión: es el lado más pequeño
// Asumo que el lado más largo es una variable global
// También si es vertical u horizontal en una variable global.

int Recorrido(int **tablero, int dimension, int inicio_F, int inicio_C)
{
    int pertenece = 0; //0: Nadie 1: Blanco 2: Negro.

    //struct Cola A;
    int *fila;
    int *columna;
    coordenadas(&fila, &columna, 1);

    int i = 0;

    int fila_actual;
    int columna_actual;

    // Iniciamos la cola con el primer elemento del tablero.
    // Primer elemento depende...

    // Añadimos a la Cola el elemento de la derecha y el elemento de abajo, si alguno es una pieza igualamos 

    while(fila[i] != NULL && columna[i] != NULL)
    {
        if (tablero[fila_actual][columna_actual] == 0) // Que NO sea una pieza o un sector ya declarado de nadie o un sector ya declarado de una pieza.
        { 
            if (fila[i] != fila_actual || columna[i] != columna_actual)
            {
                if (fila_actual + 1 != dimension)
                    //push(A, tablero[fila +1][columna_actual]); // elemento de la derecha
                if (columna_actual + 1 != dimension) // menor que...
                    //push(A, tablero[fila_actual][columna_actual + 1]);  // elemento de abajo
            }
            fila_actual = fila[i];
            columna_actual = columna[i];
        }

        if (pertenece == 0 && tablero[fila_actual][columna_actual] != 0)
        {
            pertenece = tablero[fila_actual][columna_actual];
        }

        if (pertenece != 0 && tablero[fila_actual][columna_actual] != pertenece)
        {
            // Significa que el área encontró más de una pieza
            // función de cambio para área de nadie
        }
        i++;
    }



    return 1; //temporal
}
