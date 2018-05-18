#include "mtrmul.h"

#include <stdint.h>

/* Computa o produto interno c = a * b + c entre os vetores a e b de tamanho
 * p. lda e o incremento usado para percorrer os valores de a. */
static void dotprod(double *a, uint64_t lda, double *b, double *c, uint64_t p)
{
    for (uint64_t k = 0; k < p; k++)
        *c += a(k) * b(k);
}

/*  */
static void dotprod_1x4(double *a, uint64_t lda, double *b, uint64_t ldb,
                        double *c, uint64_t ldc, uint64_t p)
{
    for (uint64_t k = 0; k < p; k++)
    {
        C(0,0) += A(0,p) * B(p,0);
        C(0,1) += A(0,p) * B(p,1);
        C(0,2) += A(0,p) * B(p,2);
        C(0,3) += A(0,p) * B(p,3);
    }
}

void mtrmul_opt(uint64_t m, uint64_t p, uint64_t n,
                double *a, uint64_t lda,
                double *b, uint64_t ldb,
                double *c, uint64_t ldc)
{
    uint64_t n_;

    for (n_ = n; n_ % 4 != 0; n_--);

    for (uint64_t j = 0; j < n_; j += 4)
        for (uint64_t i = 0; i < m; i++)
            dotprod_1x4(&A(i,0), lda, &B(0,j), ldb, &C(i,j), ldc, p);

    for (uint64_t j = n_; j < n; j++)
        for (uint64_t i = 0; i < m; i++)
            dotprod(&A(i,0), lda, &B(0,j), &C(i,j), p);
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

