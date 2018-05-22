#include "mtrio.h"
#include "mtrmem.h"
#include "mtrmul.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
    char impl;
    double *a, *b, *c;
    uint64_t m, p, n, lda, ldb, ldc;
    FILE *aFile, *bFile, *cFile;

    if (argc != 5 || (argv[1][0] != OPENMP_IMPL && argv[1][0] != PTHREAD_IMPL))
    {
        fprintf(stderr,
                "Usage: %s <implementation> <file_a> <file_b> <file_c>\n"
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
        fprintf(stderr, "Error opening file %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    bFile = fopen(argv[3], "r");
    if (bFile == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", argv[3]);
        return EXIT_FAILURE;
    }

    cFile = fopen(argv[4], "w");
    if (cFile == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", argv[4]);
        return EXIT_FAILURE;
    }

    a = readmtr(&m, &p, &lda, aFile);
    fclose(aFile);
    b = readmtr(&p, &n, &ldb, bFile);
    fclose(bFile);

    ldc = m;
    c = mtrcalloc(ldc, n);

/*printf("\nMatriz A (%" PRIu64 " por %" PRIu64 "), lda = %" PRIu64 "\n", m, p, lda);
printmtr(m, p, a, lda, stdout);
printf("\nMatriz B (%" PRIu64 " por %" PRIu64 "), ldb = %" PRIu64 "\n", p, n, ldb);
printmtr(p, n, b, ldb, stdout);*/

    if (impl == OPENMP_IMPL)
        mtrmul_opt(m, p, n, a, lda, b, ldb, c, ldc);
    else
        mtrmul_opt(m, p, n, a, lda, b, ldb, c, ldc);

/*printf("\nMatriz C (%" PRIu64 " por %" PRIu64 "), ldc = %" PRIu64 "\n", m, n, ldc);
printmtr(m, n, c, ldc, stdout);
printf("\n");*/

    printmtr(m, n, c, ldc, cFile);
    fclose(cFile);

    mtrfree(a);
    mtrfree(b);
    mtrfree(c);

    return EXIT_SUCCESS;
}
