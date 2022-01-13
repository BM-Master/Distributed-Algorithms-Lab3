#include <stdio.h>
#include <stdlib.h>

unsigned char **readFile(int dim){

    printf("en funcion readFile\n");

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

void imprimeTablero(unsigned char **tablero, int dim){
    int i,j;
    for(i = 0; i <= dim + 1; i++){
        for(j = 0; j <= dim + 1; j++){
            printf("%d ", tablero[i][j]);
        }
        printf("\n");
    }
}

void main(int argc, char *argv[]){

    int i;
    int dimension;

    unsigned char **tablero;

    scanf("%d", &dimension);

    printf("%d\n", dimension);

    tablero = readFile(dimension);

    imprimeTablero(tablero, dimension);

}
