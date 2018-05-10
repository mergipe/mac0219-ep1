#include "mtrio.h"
#include "mtrmem.h"
#include "mtrmul.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    double **mtr_A, **mtr_B, **mtr_C;
    uint64_t m, p, n;
    FILE *aFile, *bFile, *cFile;

    if (argc != 5 || (argv[1][0] != 'o' && argv[1][0] != 'p'))
    {
        printf("Usage: %s <implementation> <file_a> <file_b> <file_c>\n"
               "  where <implementation> is\n"
               "     p   for an implementation using pthreads\n"
               "     o   for an implementation using OpenMP\n",
               argv[0]);
        return EXIT_FAILURE;
    }

    aFile = fopen(argv[2], "r");
    if (aFile == NULL)
    {
        printf("Error opening file %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    bFile = fopen(argv[3], "r");
    if (bFile == NULL)
    {
        printf("Error opening file %s\n", argv[3]);
        return EXIT_FAILURE;
    }

    cFile = fopen(argv[4], "w");

    if (cFile == NULL)
    {
        printf("Error opening file %s\n", argv[4]);
        return EXIT_FAILURE;
    }

    mtr_A = readmtr(&m, &p, aFile);
    fclose(aFile);
    mtr_B = readmtr(&p, &n, bFile);
    fclose(bFile);

    mtr_C = mtrmul_naive(mtr_A, mtr_B, m, p, n);
    printmtr(mtr_C, m, n, cFile);
    fclose(cFile);

    mtrclear(mtr_A, m, p);
    mtrclear(mtr_B, p, n);
    mtrclear(mtr_C, m, n);

    return EXIT_SUCCESS;
}
