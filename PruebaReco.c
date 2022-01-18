#include<stdio.h>
#include <stdlib.h>

//[filas][columnas]

//void ingreso(int **tablero, dimension);
void mostrar(int tablero[8][8]);
void ingreso(int tablero[8][8]);
void ingreso3(int tablero[8][8]);
int ingreso_coordenadas(int tablero[8][8], int i, int j, int** fila, int **columna, int tamanio);
int Recorrido(int tablero[8][8], int inicio_F, int inicio_C);
void llenado_pertene(int pertenece, int tablero[8][8], int *fila, int *columna, int tamanio);
int ingreso_derecha(int tablero[8][8], int i, int j, int* fila, int *columna, int nt);
int ingreso_izquierda(int tablero[8][8], int i, int j, int* fila, int *columna, int nt);
int ingreso_arriba(int tablero[8][8], int i, int j, int* fila, int *columna, int nt);
int ingreso_abajo(int tablero[8][8], int i, int j, int* fila, int *columna, int nt);

int main()
{
	int dimension=8, tablero[8][8];
	
	ingreso(tablero);
	mostrar(tablero);
    int i,j;
    for(i=1;i<dimension-1; i++)
    {
        for(j=1; j<dimension-1; j++)
        {
            Recorrido(tablero, i, j);

            printf("------------------------------\n");
        }
    }
    printf("\n");
    mostrar(tablero);

	
	return 0;
}

void ingreso(int tablero[8][8])
{
	int i, j;
    ingreso3(tablero);
	for(i=0; i<8; i++)
	{
		for(j=0; j<8; j++)
		{
			if(i==1 && j==2)
                tablero[i][j]=1;
            if(i==2 && j==2)
                tablero[i][j]=1;
            if(i==3 && j==2)
                tablero[i][j]=1;
            if(i==3 && j==1)
                tablero[i][j]=1;

            if(i==2 && j==6)
                tablero[i][j]=2;
            if(i==3 && j==5)
                tablero[i][j]=2;
            if(i==4 && j==4)
                tablero[i][j]=2;
            if(i==5 && j==3)
                tablero[i][j]=2;
            if(i==6 && j==2)
                tablero[i][j]=2;

            if(i==0 || i == 7 || j==0 || j==7)
                tablero[i][j]=0;
		}
	}
}

void ingreso3(int tablero[8][8])
{
	int i, j;
	for(i=0; i<8; i++)
	{
		for(j=0; j<8; j++)
		{
			tablero[i][j]=3;
		}
	}
}

void mostrar(int tablero[8][8])
{
    int i, j;
    for(i=0; i<8; i++)
	{
		for(j=0; j<8; j++)
		{
            if (tablero[i][j]==3)
            {
                printf("[   ]");
            }
            else
                printf("[ %i ]", tablero[i][j]);
        }
        printf("\n");
    }
}

// Solo para implementar la logica de lo que contienen las piezas.

// dimension: es el lado mas pequenio.
// Asumo que el lado mas largo es una variable global.
// Tambien si es vertical u horizontal en una variable global.

struct Nodo
{
    int fila;
    int columna;
    struct Nodo *sig;
};


int Recorrido(int tablero[8][8], int inicio_F, int inicio_C)
{
    // Iniciamos la estructura de almacenamiento de los punto a recorrer.

    int *fila;
    int *columna;

    if(tablero[inicio_F][inicio_C]==3)
    {
        // Iniciamos con el primer elemento.

        fila = (int *)malloc(1*sizeof(int));
        columna = (int *)malloc(1*sizeof(int));

        int i = 1; // cantidad de elementos de los arreglos.

        int j = 0; // Avanzamos por el arreglo.

        //Ingreso del primer elemento.

        fila[0]=inicio_F; 
        columna[0]=inicio_C;

        // Inicio de la fila actual y columna actual.

        int fila_actual= inicio_F;
        int columna_actual = inicio_C;

        int pertenece = 0; //Variable que determina a quien pertenece el area
        
        //0: Inicio, 1: Blanco, 2: Negro, -1: Nadie (Definitivo).

        
        i = ingreso_coordenadas(tablero, fila[j], columna[j], &fila, &columna, i);
   
        while(j < i)
        {
            if (tablero[fila_actual][columna_actual] == 3) // Que NO sea una pieza o un sector ya declarado de nadie o un sector ya declarado de una pieza.
            { 
                if (fila[j] != fila_actual || columna[j] != columna_actual)
                {
                    i = ingreso_coordenadas(tablero, fila[j], columna[j], &fila, &columna, i);
                }

                fila_actual = fila[j];
                columna_actual = columna[j];
            }

            if (pertenece == 0 && tablero[fila_actual][columna_actual] == 1 || pertenece == 0 && tablero[fila_actual][columna_actual] == 2 )
            {
                pertenece = tablero[fila_actual][columna_actual];
            }

            if (pertenece != 0 && tablero[fila_actual][columna_actual] != pertenece)
            {
                pertenece = -1; //No pertenece ni a blanca ni a negra.
            }
            
            j++;
        }

        int u;
        for(u=0; u< i; u++)
        {
            printf("Fila: %i  Columna: %i\n", fila[u], columna[u]);
        }
    
        llenado_pertene(pertenece, tablero, fila, columna, i);

        return 1; // temporal
    }
}

void llenado_pertene(int pertenece, int tablero[8][8], int *fila, int *columna, int tamanio)
{
    int i;
    for(i=0; i < tamanio; i++)
    {
        if(pertenece== 1 && tablero[fila[i]][columna[i]] != 1)
        {
            tablero[fila[i]][columna[i]] = 4; 
        }

        if(pertenece== 2 && tablero[fila[i]][columna[i]] != 2)
        {
            tablero[fila[i]][columna[i]] = 5;
        }

        if(pertenece== -1 && tablero[fila[i]][columna[i]] != 1 && tablero[fila[i]][columna[i]] != 2)
        {
            tablero[fila[i]][columna[i]] = 9;
        }
    }
}


int ingreso_coordenadas(int tablero[8][8], int i, int j, int** fila, int **columna, int tamanio)
{
    int nt = tamanio;
    int *Nf, *Nc;
    if(tablero[i][j] == 3) 
    {
        //No tiene que ser borde
        
        if(tablero[i-1][j]!=0) // izquierda
        {
            printf("Opcion 1\n");
            printf("Tamaño: %i\n",nt);
            nt++;
            Nf = (int*) malloc((nt)*sizeof(int));
            Nc = (int*) malloc((nt)*sizeof(int));
            printf("Paso1\n");
            for (int i = 0; i < nt-1; i++){
                Nf[i] = *fila[i];
                Nc[i] = *columna[i];
                printf("ingreso\n");
            }
            printf("Paso2\n");
            Nf[-1]=i-1;
            Nc[-1]=j;
            *fila = Nf;
            *columna = Nc;
        }

        if(tablero[i+1][j] !=0) // derecha
        {   
            printf("Opcion 2\n");
            printf("Tamaño: %i\n",nt);
            nt++;
            printf("cosa1\n");
            Nf = (int*) malloc((nt)*sizeof(int));
            Nc = (int*) malloc((nt)*sizeof(int));
            printf("Paso1\n");
            for (int i = 0; i < nt-1; i++){
                Nf[i] = *fila[i];
                Nc[i] = *columna[i];
                printf("ingreso\n");
            }
            printf("Paso2\n");
            printf("cosa3\n");
            Nf[-1]=i+1;
            Nc[-1]=j;
            printf("cosa32\n");
            *fila = Nf;
            *columna = Nc;
        }

        if(tablero[i][j-1] !=0) // arriba
        {
            printf("Opcion 3\n");
            printf("Tamaño: %i\n",nt);
            nt++;
            printf("cosa1\n");
            Nf = (int*) malloc((nt)*sizeof(int));
            Nc = (int*) malloc((nt)*sizeof(int));
            printf("Paso1\n");
            for (int i = 0; i < nt-1; i++){
                Nf[i] = *fila[i];
                Nc[i] = *columna[i];
                printf("ingreso\n");
            }
            printf("Paso2\n");
            printf("cosa3\n");
            Nf[-1]=i;
            printf("cosa4\n");
            Nc[-1]=j-1;
            printf("cosa5\n");
            *fila = Nf;
            *columna = Nc;
            
        }

        if(tablero[i][j+1] !=0) // abajo
        {
            printf("Opcion 4 \n");
            printf("Tamaño: %i\n",nt);
            nt++;

            Nf = (int*) malloc((nt)*sizeof(int));
            Nc = (int*) malloc((nt)*sizeof(int));
            printf("Paso1\n");
            for (int i = 0; i < nt-1; i++){
                Nf[i] = *fila[i];
                Nc[i] = *columna[i];
                printf("ingreso\n");
            }
            printf("Paso2\n");
            Nf[-1]=i;
            Nc[-1]=j+1;
            *fila = Nf;
            *columna = Nc;
        }
        printf("Salida\n");
    }
    return nt;
}