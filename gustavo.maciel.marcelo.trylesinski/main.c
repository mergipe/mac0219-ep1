#include "mtrio.h"
#include "mtrmem.h"
#include "mtrmul.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define abs(x) (x < 0.0 ? -x : x)

double compare_matrices(uint64_t m, uint64_t n, double *a, uint64_t lda,
        double *b, uint64_t ldb)
{
    double max_diff = 0.0, diff;

    for (uint64_t j = 0; j < n; j++)
    {
        for (uint64_t i = 0; i < m; i++)
        {
            diff = abs(A(i,j) - B(i,j));
            max_diff = (diff > max_diff ? diff : max_diff);
        }
    }

    return max_diff;
}

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

    if (impl == OPENMP_IMPL)
        mtrmul_opt_o(m, p, n, a, lda, b, ldb, c, ldc);
    else
        mtrmul_opt_o(m, p, n, a, lda, b, ldb, c, ldc);

    printmtr(m, n, c, ldc, cFile);
    fclose(cFile);

double *c_;
c_ = mtrcalloc(ldc,n);
mtrmul_opt_o(m,p,n,a,lda,b,ldb,c,ldc);
printf("%le\n", compare_matrices(m, n, c, ldc, c_, ldc));
free(c_);

/*cFile = fopen(argv[4], "r");
if (cFile == NULL)
{
    fprintf(stderr, "Error opening file %s\n", argv[4]);
    return EXIT_FAILURE;
}
double *c_ = readmtr(&m, &n, &ldc, cFile);
printf("%le\n", compare_matrices(m, n, c, ldc, c_, ldc));
fclose(cFile);
mtrfree(c_);*/

    mtrfree(a);
    mtrfree(b);
    mtrfree(c);

    return EXIT_SUCCESS;
}
