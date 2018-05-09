#include "mtrmem.h"
#include "mtrmul.h"

#include <stdint.h>

double **mtrmul_naive(double **mtr_A, double **mtr_B, uint64_t m, uint64_t p,
                      uint64_t n)
{
    double **mtr_C;

    mtr_C = mtrinit(m, n);

    for (uint64_t i = 0; i < m; i++)
    {
        for (uint64_t j = 0; j < n; j++)
        {
            double sum = 0;

            for (uint64_t k = 0; k < p; k++)
                sum += mtr_A[i][k] * mtr_B[k][j];

            mtr_C[i][j] = sum;
        }
    }

    return mtr_C;
}
