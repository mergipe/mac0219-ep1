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
    register double c_00_reg, c_01_reg, c_02_reg, c_03_reg, a_0k_reg;
    double *b_p0_pntr, *b_p1_pntr, *b_p2_pntr, *b_p3_pntr;

    c_00_reg = C(0,0);
    c_01_reg = C(0,1);
    c_02_reg = C(0,2);
    c_03_reg = C(0,3);

    b_p0_pntr = &B(0,0);
    b_p1_pntr = &B(0,1);
    b_p2_pntr = &B(0,2);
    b_p3_pntr = &B(0,3);

    for (uint64_t k = 0; k < p; k++)
    {
        a_0k_reg = A(0,k);

        c_00_reg += a_0k_reg * *b_p0_pntr++;
        c_01_reg += a_0k_reg * *b_p1_pntr++;
        c_02_reg += a_0k_reg * *b_p2_pntr++;
        c_03_reg += a_0k_reg * *b_p3_pntr++;
    }

    C(0,0) = c_00_reg;
    C(0,1) = c_01_reg;
    C(0,2) = c_02_reg;
    C(0,3) = c_03_reg;
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

