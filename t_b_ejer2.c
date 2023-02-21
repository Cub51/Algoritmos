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

int *obtener_diagonal_principal(int **rand_nums, int tam_matriz, int proceso, int local_tam_matriz);
int *obtener_diagonal_secundaria(int **rand_nums, int tam_matriz, int proceso, int local_tam_matriz);
int sumar_diagonal_principal_secundaria(int *diagonal, int tam_matriz, int proceso, int local_tam_matriz);
int *obtner_num_alto(int *diagonal, int tam_matriz, int proceso, int local_tam_matriz);

int main()
{
    int count_parcial = 0, count = 0;
    int tam_matriz = 10000;
    int num_buscar = 5;

    int suma_aux_p = 0, suma_aux_s = 0;
    int suma_d_p = 0, suma_d_s = 0, mayor_d_p = 0, mayor_d_s = 0;
    int proceso, num_proc;
    int **rand_nums = (int **)calloc(tam_matriz, tam_matriz * sizeof(int *));
    for (int i = 0; i < tam_matriz; i++)
    {
        rand_nums[i] = (int *)calloc(tam_matriz, tam_matriz * sizeof(int));
    }

    int *d_principal = (int *)calloc(tam_matriz, tam_matriz * sizeof(int));
    int *d_secundaria = (int *)calloc(tam_matriz, tam_matriz * sizeof(int));
    int *d_aux = (int *)calloc(tam_matriz, tam_matriz * sizeof(int));
    int *d_aux_s = (int *)calloc(tam_matriz, tam_matriz * sizeof(int));
    int *d_aux_ordenar = (int *)calloc(tam_matriz, tam_matriz * sizeof(int));
    int *d_aux_ds = (int *)calloc(tam_matriz, tam_matriz * sizeof(int));
    int *d_aux_ordenar_s = (int *)calloc(tam_matriz, tam_matriz * sizeof(int));
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    int local_tam_matriz = tam_matriz / (int)num_proc;

    // if (proceso == 0)
    //{
    printf("- PROCESO [%d] \n", proceso);
    rand_nums = generar_matriz(tam_matriz, proceso, local_tam_matriz);

    //}

    if (proceso != 3)
    {
        d_principal = obtener_diagonal_principal(rand_nums, tam_matriz, proceso, local_tam_matriz);

        MPI_Send(
            d_principal,
            tam_matriz,
            MPI_INT,
            3,
            3,
            MPI_COMM_WORLD);
    }
    if (proceso == 3)
    {

        for (int i = 0; i < (num_proc)-1; i++)
        {
            printf("tam num = %d\n", tam_matriz);
            MPI_Recv(
                d_principal,
                tam_matriz,
                MPI_INT,
                i,
                3,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);

            for (int j = (local_tam_matriz * i); j < (local_tam_matriz * (i + 1)); j++)
            {
                d_aux[j] = d_principal[j];
            }
        }
        d_principal = obtener_diagonal_principal(rand_nums, tam_matriz, proceso, local_tam_matriz);
        for (int j = (local_tam_matriz * proceso); j < (local_tam_matriz * (proceso + 1)); j++)
        {
            d_aux[j] = d_principal[j];
        }

        printf("\nd principal\n");
        for (int i = 0; i < tam_matriz; i++)
        {
            d_principal[i] = d_aux[i];
            printf("[%d]\t", d_principal[i]);
        }
    }

    if (proceso != 3)
    {
        d_secundaria = obtener_diagonal_secundaria(rand_nums, tam_matriz, proceso, local_tam_matriz);

        MPI_Send(
            d_secundaria,
            tam_matriz,
            MPI_INT,
            3,
            3,
            MPI_COMM_WORLD);
    }
    if (proceso == 3)
    {

        for (int i = 0; i < (num_proc)-1; i++)
        {
            // printf("tam num = %d\n", tam_matriz);
            MPI_Recv(
                d_secundaria,
                tam_matriz,
                MPI_INT,
                i,
                3,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);

            for (int j = (local_tam_matriz * i); j < (local_tam_matriz * (i + 1)); j++)
            {
                d_aux_s[j] = d_secundaria[j];
            }
        }
        d_secundaria = obtener_diagonal_secundaria(rand_nums, tam_matriz, proceso, local_tam_matriz);
        for (int j = (local_tam_matriz * proceso); j < (local_tam_matriz * (proceso + 1)); j++)
        {
            d_aux_s[j] = d_secundaria[j];
        }

        printf("\nd secundaria\n");
        for (int i = 0; i < tam_matriz; i++)
        {
            d_secundaria[i] = d_aux_s[i];
            printf("[%d]\t", d_secundaria[i]);
        }
    }
    // suma de diagonal principal y secundaria
    if (proceso != 0)
    {
        printf("- PROCESO [%d] \n", proceso);
        suma_aux_p = sumar_diagonal_principal_secundaria(d_principal, tam_matriz, proceso, local_tam_matriz);
        MPI_Reduce(&suma_aux_p, &suma_d_p, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        suma_aux_s = sumar_diagonal_principal_secundaria(d_secundaria, tam_matriz, proceso, local_tam_matriz);
        MPI_Reduce(&suma_aux_s, &suma_d_s, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    if (proceso == 0)
    {
        printf("- PROCESO [%d] \n", proceso);
        suma_aux_p = sumar_diagonal_principal_secundaria(d_principal, tam_matriz, proceso, local_tam_matriz);
        MPI_Reduce(&suma_aux_p, &suma_d_p, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        suma_aux_s = sumar_diagonal_principal_secundaria(d_secundaria, tam_matriz, proceso, local_tam_matriz);
        MPI_Reduce(&suma_aux_s, &suma_d_s, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    if (proceso == 0)
    {
       
    }
    MPI_Bcast(&suma_d_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&suma_d_s, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // obtener el mayor
    if (proceso != 1)
    {
        d_aux_ordenar = obtner_num_alto(d_principal, tam_matriz, proceso, local_tam_matriz);
        MPI_Send(
            d_aux_ordenar,
            tam_matriz,
            MPI_INT,
            1,
            1,
            MPI_COMM_WORLD);
    }
    if (proceso == 1)
    {

        for (int i = 0; i < (num_proc); i++)
        {
            if (i != 1)
            {
                MPI_Recv(
                    d_aux_ordenar,
                    tam_matriz,
                    MPI_INT,
                    i,
                    1,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
                for (int j = (local_tam_matriz * i); j < (local_tam_matriz * (i + 1)); j++)
                {
                    d_aux[j] = d_aux_ordenar[j];
                }
            }
        }
        d_aux_ordenar = obtner_num_alto(d_principal, tam_matriz, proceso, local_tam_matriz);
        for (int j = (local_tam_matriz * proceso); j < (local_tam_matriz * (proceso + 1)); j++)
        {
            d_aux[j] = d_aux_ordenar[j];
        }

        int pos, aux;
        for (int i = 0; i < tam_matriz; i++)
        {
            pos = i;
            aux = d_aux[i];
            while (pos > (0) && d_aux[pos - 1] < aux)
            {
                d_aux[pos] = d_aux[pos - 1];
                pos--;
            }
            d_aux[pos] = aux;
        }

        printf("\nD mayor principal\n");
        for (int i = 0; i < tam_matriz; i++)
        {
            d_aux_ordenar[i] = d_aux[i];
            printf("[%d]\t", d_aux[i]);
        }
        mayor_d_p = d_aux_ordenar[0];
        printf("\nMayor num de principal [%d]\n", mayor_d_p);
    }
    MPI_Bcast(&mayor_d_p, 1, MPI_INT, 1, MPI_COMM_WORLD);

    if (proceso != 2)
    {
        d_aux_ordenar_s = obtner_num_alto(d_secundaria, tam_matriz, proceso, local_tam_matriz);
        MPI_Send(
            d_aux_ordenar_s,
            tam_matriz,
            MPI_INT,
            2,
            2,
            MPI_COMM_WORLD);
    }
    if (proceso == 2)
    {

        for (int i = 0; i < (num_proc); i++)
        {
            if (i != 2)
            {
                MPI_Recv(
                    d_aux_ordenar_s,
                    tam_matriz,
                    MPI_INT,
                    i,
                    2,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
                for (int j = (local_tam_matriz * i); j < (local_tam_matriz * (i + 1)); j++)
                {
                    d_aux_ds[j] = d_aux_ordenar_s[j];
                }
            }
        }
        d_aux_ordenar_s = obtner_num_alto(d_secundaria, tam_matriz, proceso, local_tam_matriz);
        for (int j = (local_tam_matriz * proceso); j < (local_tam_matriz * (proceso + 1)); j++)
        {
            d_aux_ds[j] = d_aux_ordenar_s[j];
        }

        int pos, aux;
        for (int i = 0; i < tam_matriz; i++)
        {
            pos = i;
            aux = d_aux_ds[i];
            while (pos > (0) && d_aux_ds[pos - 1] < aux)
            {
                d_aux_ds[pos] = d_aux_ds[pos - 1];
                pos--;
            }
            d_aux_ds[pos] = aux;
        }

        printf("\nD mayor secunaria\n");
        for (int i = 0; i < tam_matriz; i++)
        {
            d_aux_ordenar_s[i] = d_aux_ds[i];
            printf("[%d]\t", d_aux_ordenar_s[i]);
        }
        mayor_d_s = d_aux_ordenar_s[0];
        printf("\nMayor num de secundaria [%d]\n", mayor_d_s);
    }

    if (proceso == 2)
    {
         printf("\nSuma de D principal [%d]\n", suma_d_p);
        printf("\nSuma de D secundaria [%d]\n", suma_d_s);
        if (mayor_d_p > mayor_d_s)
        {
            printf("\nMayor numero de las diagonales: principal [%d]\n", mayor_d_p);
        }
        else if (mayor_d_p < mayor_d_s)
        {
            printf("\nMayor numero de las diagonales: secundaria [%d]\n", mayor_d_s);
        }

        if (suma_d_p > suma_d_s)
        {
            printf("\nDiagonal mas grande: principal [%d]\n", suma_d_p);
        }
        else
        {
            printf("\nDiagonal mas grande: secundaria [%d]\n", suma_d_s);
        }
    }

    /*
        if (proceso == 0)
        {
            printf("- PROCESO [%d] \n", proceso);
            count_parcial = sumar_coincidencias(rand_nums, tam_matriz, proceso, local_tam_matriz, num_buscar);
            MPI_Reduce(&count_parcial, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
            printf("Numero de coincidencias para [%d] es igual a: %d\n", num_buscar, count);
        }
    */

    // printf("[%d]\t", rand_nums[i][j]);

    /*
        d_principal = obtener_diagonal_principal(rand_nums, tam_matriz);
        d_secundaria = obtener_diagonal_secundaria(rand_nums, tam_matriz);

        mayor_d_p = obtner_num_alto(d_principal, tam_matriz);
        mayor_d_s = obtner_num_alto(d_secundaria, tam_matriz);

        suma_d_p = sumar_diagonal_principal_secundaria(d_principal, tam_matriz);
        suma_d_s = sumar_diagonal_principal_secundaria(d_secundaria, tam_matriz);

        printf("\nd principal\n");
        for (int i = 0; i < tam_matriz; i++)
        {
            printf("[%d]\t", d_principal[i]);
        }

        printf("\nd secundaria\n");
        for (int i = 0; i < tam_matriz; i++)
        {
            printf("[%d]\t", d_secundaria[i]);
        }

        printf("\nMayor num d principal [%d]\n", mayor_d_p);
        printf("Mayor num d secundaria [%d]\n", mayor_d_s);
        printf("Sum d principal [%d]\n", suma_d_p);
        printf("Sum d secundaria [%d]\n\n", suma_d_s);

        if (mayor_d_p > mayor_d_s)
        {
            printf("\nMayor numero de las diagonales: principal [%d]\n", mayor_d_p);
        }else{
            printf("\nMayor numero de las diagonales: secundaria [%d]\n", mayor_d_s);
        }
        if (suma_d_p > suma_d_s)
        {
            printf("\nDiagonal mas grande: principal [%d]\n", suma_d_p);
        }else{
            printf("\nDiagonal mas grande: secundaria [%d]\n", suma_d_s);
        }
        */
    free(d_secundaria);
    free(d_principal);
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
            rand_nums[i][j] = rand() % 100; // rand maximo de 99
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

int *obtener_diagonal_principal(int **rand_nums, int tam_matriz, int proceso, int local_tam_matriz)
{
    int *diagonal_principal = (int *)calloc(tam_matriz, tam_matriz * sizeof(int));

    for (int i = (local_tam_matriz * proceso); i < (local_tam_matriz * (proceso + 1)); i++)
    {
        for (int j = 0; j < tam_matriz; j++)
        {
            if (i == j)
            {
                diagonal_principal[i] = rand_nums[i][j];
            }
        }
    }
    return diagonal_principal;
}

int *obtener_diagonal_secundaria(int **rand_nums, int tam_matriz, int proceso, int local_tam_matriz)
{
    int *diagonal_secundarua = (int *)malloc(tam_matriz * sizeof(int));

    for (int i = (local_tam_matriz * proceso); i < (local_tam_matriz * (proceso + 1)); i++)
    {
        for (int j = 0; j < tam_matriz; j++)
        {
            if ((i + j) == tam_matriz - 1)
            {
                diagonal_secundarua[i] = rand_nums[i][j];
            }
        }
    }
    return diagonal_secundarua;
}

int sumar_diagonal_principal_secundaria(int *diagonal, int tam_matriz, int proceso, int local_tam_matriz)
{
    int suma_d_p = 0;

    for (int i = (local_tam_matriz * proceso); i < (local_tam_matriz * (proceso + 1)); i++)
    {
        suma_d_p = suma_d_p + (int)diagonal[i];
    }
    return suma_d_p;
}

int *obtner_num_alto(int *diagonal, int tam_matriz, int proceso, int local_tam_matriz)
{
    // int num_alto = 0;

    // insercion
    int pos, aux;
    for (int i = (local_tam_matriz * proceso); i < (local_tam_matriz * (proceso + 1)); i++)
    {
        pos = i;
        aux = diagonal[i];
        while (pos > (local_tam_matriz * proceso) && diagonal[pos - 1] < aux)
        {
            diagonal[pos] = diagonal[pos - 1];
            pos--;
        }
        diagonal[pos] = aux;
    }

    // num_alto = diagonal[0];

    return diagonal;
}