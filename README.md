# Distributed-Algorithms-Lab3

# Compilación
mpicc main.c -o t.exe
mpirun -np K ./t.exe < tabvalido.txt

Obs: El K es un número entre 2 y la dimensión (inclusive) del tablero a procesar, en el caso del archivo es 19
