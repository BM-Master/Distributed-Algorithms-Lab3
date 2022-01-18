#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
    TODO:
        1- Implementar la cola a cada subproceso
        2- Realizar el análisis del tablero
        3- Obtener un resultado coherente al juntar los resultados de cada proceso
        4- Implementar la partición horizontal
*/

int borde_esquina(int i, int j, int **tablero, int dimension);
unsigned char **leerTablero(int dim);
unsigned char** leerTableroFinal(int dim);
unsigned char **crearSubTableroVertical(unsigned char **tablero, int colInicio, int colFin, int filas);
unsigned char **creaTableroVacio(int filas, int columnas);
unsigned char **creaTableroVacioFinal(int dim);
void imprimeTablero(unsigned char **tablero, int filas, int columnas);
void imprimeTableroConCeros(unsigned char **tablero, int filas, int columnas);
void reparteTablero(unsigned char **tablero, int dim, int numProcesos);
void testRecover();

MPI_Status status;

// 0 -> bordes
// 1 -> ficha negra
// 2 -> ficha blanca
// 3 -> lugar vacío

#define MASTER 0
#define TAG_MATRIZ 1 // traspaso de submatriz a cada proceso
#define TAG_RESULTADOS 3 // traspaso de resultados {0, 1} con 0 que ganó el área la ficha negra y 1 que ganó la ficha blanca
#define TAG_DIMENSION 2 // traspaso de la dimensión del tablero de cada proceso

unsigned int whoami, hmaw;

int colInicioMaster, colMaster, dMaster;

unsigned char** tab;

int main(int argc, char *argv[])
{
    int dimension;
    unsigned char **tablero;

    unsigned int me;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

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
        tab = leerTableroFinal(dimension);
        dMaster = dimension;
        imprimeTableroConCeros(tab, dimension, dimension);

        reparteTablero(tab, dimension, hmaw); // parece que esta función va en el testRecover dentro del master
    }

    MPI_Barrier(MPI_COMM_WORLD);

    testRecover();

    MPI_Finalize();

    return 0;
}

// TESTEO DE ENVÍO DE MENSAJES
void testRecover()
{
    int dim2 = 8;
    int a;
    int a2;
    int a3;
    unsigned char **c;

    int dimensiones[2];

    if (whoami == MASTER)
    {
        printf("soy el proceso master\n");
        c = crearSubTableroVertical(tab, colInicioMaster, colMaster, dMaster);
        imprimeTablero(c, dMaster, colMaster);

    }
    else
    {

        printf("soy el proceso %d\n", whoami);
        int i, j;

        printf("Recibiendo dimensiones . . . \n");
        MPI_Recv(dimensiones, 2, MPI_INT, MASTER, TAG_DIMENSION, MPI_COMM_WORLD, &status);
        printf("dimensions: %d %d\n", dimensiones[0], dimensiones[1]);

        c = creaTableroVacio(dimensiones[0], dimensiones[1]);

        printf("Recibiendo tablero . . . \n");
        for (i = 0; i < dimensiones[0]; i++)
        {
            MPI_Recv(c[i], dimensiones[1], MPI_UNSIGNED_CHAR, MASTER, TAG_MATRIZ, MPI_COMM_WORLD, &status);
        }

        fflush(stdout);
        imprimeTablero(c, dimensiones[0], dimensiones[1]);
    }
}

/*
    Uso: Lee un archivo de texto con la información del tablero y devuelve una matriz
         con la información pero con ceros extras.
    Entrada: dimensión del tablero más dos
    Devuelve: tablero con las posiciones de las fichas negras y blancas
*/

unsigned char **leerTablero(int dim)
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

unsigned char** leerTableroFinal(int dim)
{
    int cantidadNegras, cantidadBlancas;
    unsigned char **tablero;
    tablero = creaTableroVacioFinal(dim);
    char t[5];
    char temp;
    int i, j, count;
    int fila;
    int col;

    count = 0;
    scanf("%d", &cantidadNegras);
    printf("%d\n", cantidadNegras);

    printf("hola");

    while (count != cantidadNegras)
    {
        scanf("%c", &temp);
        scanf("%[^\n]", t);
        fila = atoi(&t[0]);
        col = atoi(&t[2]);
        fflush(stdin);

        t[0] = 0;
        t[1] = 0;
        t[2] = 0;
        t[3] = 0;
        t[4] = 0;

        tablero[fila][col] = 1;
        count++;
    }
    count = 0;
    scanf("%d", &cantidadBlancas);
    printf("%d\n", cantidadBlancas);

    while (count != cantidadBlancas)
    {
        scanf("%c", &temp);
        scanf("%[^\n]", t);      
        fila = atoi(&t[0]); 
        col = atoi(&t[2]);
        fflush(stdin);

        printf("fila: %d ,col: %d \n", fila, col);

        t[0] = 0;
        t[1] = 0;
        t[2] = 0;
        t[3] = 0;
        t[4] = 0;

        tablero[fila][col] = 2;
        count++;
    }

    for(i = 1; i<=dim; i++){
        for(j = 1; j<=dim; j++){
            if(tablero[i][j] != 1 && tablero[i][j] != 2){
                tablero[i][j] = 3;
            }
        }
    }

    return tablero;
}

unsigned char **creaTableroVacioFinal(int dim)
{
    unsigned char **tablero;
    int i;

    tablero = calloc(dim + 2, sizeof(unsigned char *)); // dim + 2 para no preocuparse de los bordes
    for (i = 0; i < dim + 2; i++)
    {
        tablero[i] = calloc(dim + 2, sizeof(unsigned char));
    }

    return tablero;
}


/*
    Uso: Crea una matriz de ceros con la dimensión indicada
    Entrada: dimensión del tablero (filas y columnas)
    Devuelve: matriz de ceros
*/

unsigned char **creaTableroVacio(int filas, int columnas)
{
    unsigned char **tablero;
    int i;

    tablero = calloc(filas, sizeof(unsigned char *));
    for (i = 0; i < filas; i++)
    {
        tablero[i] = calloc(columnas, sizeof(unsigned char));
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
    for (i = 0; i < filas; i++)
    {
        for (j = 0; j < columnas; j++)
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
    int dimPorProceso = dim / nprocesos; // columnas que utilizará cada proceso
    int resto = dim % nprocesos;         // resto de la división
    int i, j;
    int contInicio = 0;
    int contFin = 0;
    int flagprocs = nprocesos;

    int arrDimensiones[2];
    int trabajoPorProceso[nprocesos];

    unsigned char **testSubTablero;

    for (i = 0; i < nprocesos; i++) // repartimos tareas
    {
        if (resto != 0)
        {
            trabajoPorProceso[i] = dimPorProceso + 1;
            resto--;
        }
        else
        {
            trabajoPorProceso[i] = dimPorProceso;
        }
    }

    // for (i = 0; i < nprocesos; i++)
    // {
    //     printf("Proceso %d: %d\n", i, trabajoPorProceso[i]);
    // }

    contInicio = 0;                     // para el primer proceso
    contFin = trabajoPorProceso[0] - 1; // para el primer proceso

    colInicioMaster = contInicio;
    colMaster = contFin;

    arrDimensiones[0] = dim + 2; // las dimensiones de la fila siempre serán +2 para no preocuparse de los bordes

    // printf("inicio y contfin: %d %d\n", contInicio, contFin);

    for (i = 1; i < nprocesos; i++) // comienza del 1 porque se omite el proceso 0
    {
        // printf("trabajoPorProceso[%d] = %d\n", i, trabajoPorProceso[i]);
        contFin += trabajoPorProceso[i];
        contInicio += trabajoPorProceso[i - 1];
        // printf("contInicio y contFin %d %d\n", contInicio, contFin);
        // printf("contfin - contInicio + 2 =  %d\n", contFin - contInicio + 3);
        testSubTablero = crearSubTableroVertical(tablero, contInicio, contFin, dim);

        arrDimensiones[1] = contFin - contInicio + 3;

        // printf("arrdimensiones %d %d\n", arrDimensiones[0], arrDimensiones[1]);
        MPI_Send(arrDimensiones, 2, MPI_INT, i, TAG_DIMENSION, MPI_COMM_WORLD);

        // comienzo de envío de la matriz por trozos
        printf("Comenzando proceso de envío . . .\n");
        for (j = 0; j < arrDimensiones[0]; j++)
        {
            MPI_Send(testSubTablero[j], arrDimensiones[1], MPI_UNSIGNED_CHAR, i, TAG_MATRIZ, MPI_COMM_WORLD);
        }
    }
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
    int i, j;
    int k = colFin - colInicio + 1;

    subTablero = calloc(filas + 2, sizeof(unsigned char *)); // filas
    for (i = 0; i < filas + 2; i++)
    {
        subTablero[i] = calloc(k + 2, sizeof(unsigned char)); // columnas
    }

    for (i = 1; i <= filas; i++)
    {
        for (j = 1; j <= k; j++)
        {
            subTablero[i][j] = tablero[i][j + colInicio];
        }
    }

    // printf("subtablero: \n");
    // imprimeTableroConCeros(subTablero, filas, k);

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
