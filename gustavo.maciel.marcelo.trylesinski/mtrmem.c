#include "mtrmem.h"

#include <stdint.h>
#include <stdlib.h>

double **mtrinit(uint64_t m, uint64_t n)
{
    double **mtr;

    mtr = malloc(m * sizeof (double *));
    for (uint64_t i = 0; i < m; i++)
        mtr[i] = malloc(n * sizeof (double));

    return mtr;
}

void mtrclear(double **mtr, uint64_t m, uint64_t n)
{
    for (uint64_t i = 0; i < m; i++)
        free(mtr[i]);
    free(mtr);
}
