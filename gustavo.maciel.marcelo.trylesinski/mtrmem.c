#include "mtrmem.h"

#include <stdint.h>
#include <xmmintrin.h>

/* Macro para acessar uma posicao (i,j) da matriz mtr */
#define M(i,j) mtr[(j) * ld + (i)]

double *mtralloc(uint64_t ld, uint64_t od)
{
    double *mtr;

    mtr = (double *) _mm_malloc(ld * od * sizeof (double), ALIGN_SIZE);

    return mtr;
}

double *mtrcalloc(uint64_t ld, uint64_t od)
{
    double *mtr;

    mtr = mtralloc(ld, od);

    for (uint64_t j = 0; j < od; j++)
        for (uint64_t i = 0; i < ld; i++)
            M(i,j) = 0;

    return mtr;
}

void mtrfree(double *mtr)
{
    _mm_free(mtr);
}
