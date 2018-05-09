#include "matrixio.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    uint64_t m, p, n;
    double **A, **B, **C;
    FILE *AFile, *BFile, *CFile;

    if (argc != 4)
    {
        printf("Usage: %s <path_to_A> <path_to_B> <path_to_C>\n", argv[0]);
        return EXIT_FAILURE;
    }

    AFile = fopen(argv[1], "r");
    if (AFile == NULL)
    {
        printf("Error opening file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    BFile = fopen(argv[2], "r");
    if (BFile == NULL)
    {
        printf("Error opening file %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    CFile = fopen(argv[3], "w");

    if (CFile == NULL)
    {
        printf("Error opening file %s\n", argv[3]);
        return EXIT_FAILURE;
    }

    A = read_matrix(&m, &p, AFile);
    fclose(AFile);
    B = read_matrix(&p, &n, BFile);
    fclose(BFile);

    C = malloc(m * sizeof (double *));
    for (uint64_t i = 0; i < m; i++)
        C[i] = malloc(n * sizeof (double));

    /* Faz os cálculos... */

    print_matrix(C, m, n, CFile);
    fclose(CFile);

    for (uint64_t i = 0; i < m; i++)
    {
        free(A[i]);
        free(C[i]);
    }
    free(A);
    free(C);

    for (uint64_t i = 0; i < p; i++)
        free(B[i]);
    free(B);

    return EXIT_SUCCESS;
}
