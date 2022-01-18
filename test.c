#include <stdio.h>
#include <stdlib.h>

unsigned char **creaTableroVacio(int dim)
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

void imprimeTablero(unsigned char **tablero, int filas, int columnas)
{
    int i, j;
    for (i = 0; i <= filas + 1; i++)
    {
        for (j = 0; j <= columnas; j++)
        {
            printf("%d ", tablero[i][j]);
        }
        printf("\n");
    }
}

unsigned char** leerTableroFinal(int dim)
{
    int cantidadNegras, cantidadBlancas;
    unsigned char **tablero;
    tablero = creaTableroVacio(dim);
    char t[5];
    char temp;
    int i, j, count;
    int fila;
    int col;
    int x;

    count = 0;
    scanf("%d", &cantidadNegras);
    printf("%d\n", cantidadNegras);

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

    for(i = 1; i<dim; i++){
        for(j = 1; j<dim; j++){
            if(tablero[i][j] != 1 && tablero[i][j] != 2){
                tablero[i][j] = 3;
            }
        }
    }

    return tablero;
}

void main(int argc, char *argv[])
{

    int i;
    int dimension;

    unsigned char **tablero;

    scanf("%d", &dimension);

    printf("%d\n", dimension);

    tablero = leerTableroFinal(dimension);
    imprimeTablero(tablero, dimension, dimension);
}
