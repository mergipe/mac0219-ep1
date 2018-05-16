#include "mtrmul.h"

#include <stdint.h>

#define A(i,j) a[j * m + i]
#define B(i,j) b[j * p + i]
#define C(i,j) c[j * m + i]

void mtrmul_naive(double *a, double *b, double *c,
                  uint64_t m, uint64_t p, uint64_t n)
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

void mtrmul_opt(double *a, double *b, double *c,
                uint64_t m, uint64_t p, uint64_t n)
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
