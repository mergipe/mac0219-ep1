#include "../mtrio.h"
#include "../mtrmem.h"
#include "../mtrmul.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define PFIRST 50
#define PLAST  1000
#define PINC   50

#define NREPEATS 3

#define A(i,j) a[j * lda + i]
#define B(i,j) b[j * ldb + i]
#define abs(x) (x < 0.0 ? -x : x)

double dclock();

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

int main()
{
    double *a, *b, *c, *cold, *cref;
    uint64_t m, p, n, lda, ldb, ldc;
    double gflops, dtime, dtime_best, diff;

/*FILE *aFile, *bFile;
aFile = fopen("data/A", "w");
bFile = fopen("data/B", "w");
a = mtralloc(1000, 1000);
b = mtralloc(1000, 1000);
random_matrix(1000, 1000, a, 1000);
random_matrix(1000, 1000, b, 1000);
printmtr(1000, 1000, a, 1000, aFile);
printmtr(1000, 1000, b, 1000, bFile);
fclose(aFile);
fclose(bFile);*/

    printf("MY_MMult = [\n");

    for (int k = PFIRST; k <= PLAST; k += PINC)
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

        for (int rep = 0; rep < NREPEATS; rep++)
        {
            copy_matrix(m, n, cold, ldc, c, ldc);

            dtime = dclock();

            mtrmul_opt_o(m, p, n, a, lda, b, ldb, c, ldc);

            dtime = dclock() - dtime;

            if (rep == 0)
                dtime_best = dtime;
            else
                dtime_best = (dtime < dtime_best ? dtime : dtime_best);
        }

        diff = compare_matrices(m, n, c, ldc, cref, ldc);

        printf("%d %le %le %le\n", k, gflops / dtime_best, diff, dtime_best);
        fflush(stdout);

        mtrfree(a);
        mtrfree(b);
        mtrfree(c);
        mtrfree(cold);
        mtrfree(cref);
    }

    printf("];\n");

    return EXIT_SUCCESS;
}
