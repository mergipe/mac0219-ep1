#include "../mtrio.h"
#include "../mtrmem.h"
#include "../mtrmul.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PFIRST 50
#define PLAST  1000
#define PINC   50

#define NREPEATS 5

#define A(i,j) a[j * lda + i]
#define B(i,j) b[j * ldb + i]

void random_matrix(uint64_t m, uint64_t n, double *a, uint64_t lda)
{
    double drand48();

    for (uint64_t j = 0; j < n; j++)
        for (uint64_t i = 0; i < m; i++)
            A(i,j) = 2.0 * drand48() - 1.0;
}

void copy_matrix(uint64_t m, uint64_t n, double *a, uint64_t lda, double *b,
        uint64_t ldb)
{
    for (uint64_t j = 0; j < n; j++)
        for (uint64_t i = 0; i < m; i++)
            B(i,j) = A(i,j);
}

int main()
{
    double *a, *b, *c, *cold, *cref;
    uint64_t m, p, n, k, lda, ldb, ldc;
    double gflops;

/*FILE *aFile, *bFile;
aFile = fopen("data/A", "w");
bFile = fopen("data/B", "w");
a = mtralloc(4, 3);
b = mtralloc(3, 5);
random_matrix(4, 3, a, 4);
random_matrix(3, 5, b, 3);
printmtr(4, 3, a, 3, aFile);
printmtr(3, 5, b, 3, bFile);
fclose(aFile);
fclose(bFile);*/

    printf("MY_MMult = [\n");

    for (k = PFIRST; k <= PLAST; k += PINC)
    {
        m = p = n = k;

        lda = m;
        ldb = p;
        ldc = m;

        gflops = 2.0 * m * p * n * 1.0e-09;

        a = mtralloc(lda, p);
        b = mtralloc(ldb, n);
        c = mtralloc(ldc, n);
        cold = mtralloc(ldc, n);
        cref = mtralloc(ldc, n);

        random_matrix(m, p, a, lda);
        random_matrix(p, n, b, ldb);
        random_matrix(m, n, cold, ldc);

        copy_matrix(m, n, cold, ldc, cref, ldc);

        mtrmul_naive(m, p, n, a, lda, b, ldb, cref, ldc);
    }

    return EXIT_SUCCESS;
}
