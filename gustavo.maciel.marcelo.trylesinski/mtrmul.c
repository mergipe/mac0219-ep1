#include "mtrmul.h"

#include <stdint.h>

/* Computa o produto interno c = a * b + c entre os vetores a e b de tamanho
 * p. lda e o incremento usado para percorrer os valores de a. */
static void dotprod(double *a, uint64_t lda, double *b, double *c, uint64_t p)
{
    for (uint64_t k = 0; k < p; k++)
        *c += a(k) * b(k);
}

static void dotprod_1x4(double *a, uint64_t lda, double *b, uint64_t ldb,
                        double *c, uint64_t ldc, uint64_t p)
{
    dotprod(&A(0,0), lda, &B(0,0), &C(0,0), p);
    dotprod(&A(0,0), lda, &B(0,1), &C(0,1), p);
    dotprod(&A(0,0), lda, &B(0,2), &C(0,2), p);
    dotprod(&A(0,0), lda, &B(0,3), &C(0,3), p);
}

void mtrmul_opt(uint64_t m, uint64_t p, uint64_t n,
                double *a, uint64_t lda,
                double *b, uint64_t ldb,
                double *c, uint64_t ldc)
{
    for (uint64_t j = 0; j < n; j += 4)
        for (uint64_t i = 0; i < m; i++)
            dotprod_1x4(&A(i,0), lda, &B(0,j), ldb, &C(i,j), ldc, p);
}

void mtrmul_naive(uint64_t m, uint64_t p, uint64_t n,
                  double *a, uint64_t lda,
                  double *b, uint64_t ldb,
                  double *c, uint64_t ldc)
{
    for (uint64_t j = 0; j < n; j++)
        for (uint64_t i = 0; i < m; i++)
            for (uint64_t k = 0; k < p; k++)
                C(i,j) = C(i,j) + A(i,k) * B(k,j);
}

