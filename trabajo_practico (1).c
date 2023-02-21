/*
Ejercicio 1
De una matriz 100000X100000 se debe buscar el numero de veces que aparece un número dentro de la matriz.

Ejercicio 2
De una matriz 100000X100000 se debe extraer la diagonal y de ahi hacer las siguientes operaciones:
Calcular la suma de las diagonales (por separado)
Calcular el valor mas alto de las diagonales (por separado)

Calcular la suma de las diagonales (por separado)
Calcular el valor mas alto de las diagonales (por separado)

Cual tiene el valor mas alto de las diagonales
Cual diagonal es mas grande

Para ambos ejercicios se necesita:
Implementar todo en MPI e ingresar datos por teclado
Un video explicando el funcionamiento del código
El código en git

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <mpi.h>

// de una matriz 100000X100000 se debe buscar el numero de veces que aparece un número dentro de la matriz.
int **generar_matriz(int tam_matriz, int proceso, int local_tam_matriz);
int sumar_coincidencias(int **rand_nums, int tam_matriz, int proceso, int local_tam_matriz, int num_buscar);

int main()
{
    int count_parcial = 0, count = 0;
    int tam_matriz = 10000; // 20000
    int num_buscar = 5;

    int suma_d_p = 0, suma_d_s = 0, mayor_d_p = 0, mayor_d_s = 0;
    int proceso, num_proc;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    int local_tam_matriz = tam_matriz / (int)num_proc;
    // int **rand_nums;
    int **rand_nums = (int **)calloc(tam_matriz, tam_matriz * sizeof(int *));
    for (int i = 0; i < tam_matriz; i++)
    {
        rand_nums[i] = (int *)calloc(tam_matriz, tam_matriz * sizeof(int));
        //  for (int j = 0; j < tam_matriz; j++){
        //  rand_nums[i][j] =  10; // rand maximo de 99
    }
    // int **nums_total = (int **)malloc(tam_matriz * sizeof(int *));
    // int *d_principal = (int *)malloc(tam_matriz * sizeof(int));
    // int *d_secundaria = (int *)malloc(tam_matriz * sizeof(int));

    if (proceso != 0)
    {
        printf("- PROCESO [%d] \n", proceso);
        count_parcial = sumar_coincidencias(rand_nums, tam_matriz, proceso, local_tam_matriz, num_buscar);

        MPI_Reduce(&count_parcial, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        printf("win\n");
    }
    if (proceso == 0)
    {
        printf("Ingrese el numero del cual quiere las coincidencias # :\n ");
        scanf("%d", &num_buscar);
    }
    MPI_Bcast(&num_buscar, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (proceso == 0)
    {
        printf("- PROCESO [%d] \n", proceso);

        rand_nums = generar_matriz(tam_matriz, proceso, local_tam_matriz);
    }

    MPI_Bcast(rand_nums, tam_matriz * tam_matriz, MPI_INT, 0, MPI_COMM_WORLD);

    if (proceso == 0)
    {
        printf("- PROCESO [%d] \n", proceso);
        count_parcial = sumar_coincidencias(rand_nums, tam_matriz, proceso, local_tam_matriz, num_buscar);
        MPI_Reduce(&count_parcial, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        printf("Numero de coincidencias para [%d] es igual a: %d\n", num_buscar, count);
    }

    if (proceso != 3)
    {
    }

    if (proceso == 3)
    {
    }

    printf("\n");
    free(rand_nums);
    MPI_Finalize();
    return 0;
}

int **generar_matriz(int tam_matriz, int proceso, int local_tam_matriz)
{
    int **rand_nums = (int **)calloc(tam_matriz, tam_matriz * sizeof(int *));
    // int** rand_nums[tam_matriz][tam_matriz];
    srand(time(NULL));

    // for (int i = (local_tam_matriz * proceso); i < (local_tam_matriz * (proceso + 1)); i++)
    for (int i = 0; i < tam_matriz; i++)
    {
        // rand_nums[i] = (int *)malloc(tam_matriz * sizeof(int ));
        rand_nums[i] = (int *)calloc(tam_matriz, tam_matriz * sizeof(int));
        for (int j = 0; j < tam_matriz; j++)
        {
            rand_nums[i][j] = rand() % 10; // rand maximo de 99
        }
    }

    return rand_nums;
}

int sumar_coincidencias(int **rand_nums, int tam_matriz, int proceso, int local_tam_matriz, int num_buscar)
{
    int suma_coincidencias = 00;

    // for (int i = 0; i < tam_matriz; i++)
    for (int i = (local_tam_matriz * proceso); i < (local_tam_matriz * (proceso + 1)); i++)
    {
        for (int j = 0; j < tam_matriz; j++)
        {
            // printf("ss: %d", rand_nums[i][j]);
            if (num_buscar == rand_nums[i][j])
            {
                suma_coincidencias++;
            }
        }
    }
    return suma_coincidencias;
}