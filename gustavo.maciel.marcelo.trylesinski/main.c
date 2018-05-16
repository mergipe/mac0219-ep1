#include "mtrio.h"
#include "mtrmem.h"
#include "mtrmul.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char impl;
    double *a, *b, *c;
    uint64_t m, p, n;
    FILE *aFile, *bFile, *cFile;

    if (argc != 5 || (argv[1][0] != OPENMP_IMPL && argv[1][0] != PTHREAD_IMPL))
    {
        printf("Usage: %s <implementation> <file_a> <file_b> <file_c>\n"
               "  where <implementation> is\n"
               "     p   for an implementation using pthreads\n"
               "     o   for an implementation using OpenMP\n",
               argv[0]);
        return EXIT_FAILURE;
    }

    impl = argv[1][0];

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

    a = readmtr(&m, &p, aFile);
    fclose(aFile);
    b = readmtr(&p, &n, bFile);
    fclose(bFile);
    c = mtrcalloc(m, n);

printmtr(a, m, p, stdout);
printmtr(b, p, n, stdout);

    if (impl == OPENMP_IMPL)
        mtrmul_naive(a, b, c, m, p, n);
    else
        mtrmul_naive(a, b, c, m, p, n);

printmtr(c, m, n, stdout);
    printmtr(c, m, n, cFile);
    fclose(cFile);

    mtrfree(a);
    mtrfree(b);
    mtrfree(c);

    return EXIT_SUCCESS;
}
