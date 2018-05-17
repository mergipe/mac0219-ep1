#include "mtrmul.h"

#include <stdint.h>

#define A(i,j) a[j * lda + i]
#define B(i,j) b[j * ldb + i]
#define C(i,j) c[j * ldc + i]

void mtrmul_naive(uint64_t m, uint64_t p, uint64_t n,
                  double *a, uint64_t lda,
                  double *b, uint64_t ldb,
                  double *c, uint64_t ldc)
{
    for (uint64_t i = 0; i < m; i++)
    {
        for (uint64_t j = 0; j < n; j++)
        {
            for (uint64_t k = 0; k < p; k++)
            {
                C(i,j) = C(i,j) + A(i,k) * B(k,j);
            }
        }
    }
}

void mtrmul_opt(uint64_t m, uint64_t p, uint64_t n,
                double *a, uint64_t lda,
                double *b, uint64_t ldb,
                double *c, uint64_t ldc)
{
    for (uint64_t i = 0; i < m; i++)
    {
        for (uint64_t j = 0; j < n; j++)
        {
            for (uint64_t k = 0; k < p; k++)
            {
                C(i,j) = C(i,j) + A(i,k) * B(k,j);
            }
        }
    }

}
