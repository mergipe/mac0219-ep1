#include "mtrmem.h"

#include <stdint.h>
#include <stdlib.h>

#define M(i,j) mtr[j * m + i]

double *mtralloc(uint64_t m, uint64_t n)
{
    double *mtr;

    mtr = (double *) malloc(m * n * sizeof (double));

    return mtr;
}

double *mtrcalloc(uint64_t m, uint64_t n)
{
    double *mtr;

    mtr = mtralloc(m, n);

    for (uint64_t i = 0; i < m; i++)
        for (uint64_t j = 0; j < n; j++)
            M(i,j) = 0;

    return mtr;
}

void mtrfree(double *mtr)
{
    free(mtr);
}
