#include "mtrmul.h"
#include "mtrmem.h"

#include <stdint.h>
#include <omp.h>

/* Computa o produto interno c = a * b + c entre os vetores a e b de tamanho
 * p. lda e o incremento usado para percorrer os valores de a. */
static void dotprod(double *a, uint64_t lda, double *b, double *c, uint64_t p)
{
    for (uint64_t k = 0; k < p; k++)
        *c += a(k) * b(k);
}

/* Computa 4x4 produtos internos...  */
static void dotprod_4x4(double *a_, uint64_t lda, double *b, uint64_t ldb,
                        double *c, uint64_t ldc, uint64_t p)
{
    register double c_00_reg, c_01_reg, c_02_reg, c_03_reg,
                    c_10_reg, c_11_reg, c_12_reg, c_13_reg,
                    c_20_reg, c_21_reg, c_22_reg, c_23_reg,
                    c_30_reg, c_31_reg, c_32_reg, c_33_reg,

                    a_0k_reg,
                    a_1k_reg,
                    a_2k_reg,
                    a_3k_reg;

    double *b_k0_pntr, *b_k1_pntr, *b_k2_pntr, *b_k3_pntr;

    c_00_reg = C(0,0); c_01_reg = C(0,1); c_02_reg = C(0,2); c_03_reg = C(0,3);
    c_10_reg = C(1,0); c_11_reg = C(1,1); c_12_reg = C(1,2); c_13_reg = C(1,3);
    c_20_reg = C(2,0); c_21_reg = C(2,1); c_22_reg = C(2,2); c_23_reg = C(2,3);
    c_30_reg = C(3,0); c_31_reg = C(3,1); c_32_reg = C(3,2); c_33_reg = C(3,3);

    for (uint64_t k = 0; k < p; k++)
    {
        a_0k_reg = A_(0,k);
        a_1k_reg = A_(1,k);
        a_2k_reg = A_(2,k);
        a_3k_reg = A_(3,k);

        b_k0_pntr = &B(k,0);
        b_k1_pntr = &B(k,1);
        b_k2_pntr = &B(k,2);
        b_k3_pntr = &B(k,3);

        c_00_reg += a_0k_reg * *b_k0_pntr;
        c_01_reg += a_0k_reg * *b_k1_pntr;
        c_02_reg += a_0k_reg * *b_k2_pntr;
        c_03_reg += a_0k_reg * *b_k3_pntr;

        c_10_reg += a_1k_reg * *b_k0_pntr;
        c_11_reg += a_1k_reg * *b_k1_pntr;
        c_12_reg += a_1k_reg * *b_k2_pntr;
        c_13_reg += a_1k_reg * *b_k3_pntr;

        c_20_reg += a_2k_reg * *b_k0_pntr;
        c_21_reg += a_2k_reg * *b_k1_pntr;
        c_22_reg += a_2k_reg * *b_k2_pntr;
        c_23_reg += a_2k_reg * *b_k3_pntr;

        c_30_reg += a_3k_reg * *b_k0_pntr++;
        c_31_reg += a_3k_reg * *b_k1_pntr++;
        c_32_reg += a_3k_reg * *b_k2_pntr++;
        c_33_reg += a_3k_reg * *b_k3_pntr++;
    }

    C(0,0) = c_00_reg; C(0,1) = c_01_reg; C(0,2) = c_02_reg; C(0,3) = c_03_reg;
    C(1,0) = c_10_reg; C(1,1) = c_11_reg; C(1,2) = c_12_reg; C(1,3) = c_13_reg;
    C(2,0) = c_20_reg; C(2,1) = c_21_reg; C(2,2) = c_22_reg; C(2,3) = c_23_reg;
    C(3,0) = c_30_reg; C(3,1) = c_31_reg; C(3,2) = c_32_reg; C(3,3) = c_33_reg;
}

void mtrmul_opt_o(uint64_t m, uint64_t p, uint64_t n,
                  double *a, uint64_t lda,
                  double *b, uint64_t ldb,
                  double *c, uint64_t ldc)
{
    uint64_t m_, n_;
    double *a_;

    a_ = mtralloc(lda, p);

    for (uint64_t i = 0; i < m; i++)
        for (uint64_t j = 0; j < p; j++)
            A_(i,j) = A(i,j);

    for (n_ = n; n_ % 4 != 0; n_--);
    for (m_ = m; m_ % 4 != 0; m_--); 

#pragma omp parallel for
    for (uint64_t j = 0; j < n_; j += 4)
        for (uint64_t i = 0; i < m_; i += 4)
            dotprod_4x4(&A_(i,0), lda, &B(0,j), ldb, &C(i,j), ldc, p);

    for (uint64_t j = n_; j < n; j++)
        for (uint64_t i = 0; i < m; i++)
            dotprod(&A_(i,0), lda, &B(0,j), &C(i,j), p);

    for (uint64_t i = m_; i < m; i++)
        for (uint64_t j = 0; j < n_; j++)
            dotprod(&A_(i,0), lda, &B(0,j), &C(i,j), p);

    mtrfree(a_);
}

void mtrmul_opt_p(uint64_t m, uint64_t p, uint64_t n,
                  double *a, uint64_t lda,
                  double *b, uint64_t ldb,
                  double *c, uint64_t ldc)
{
    uint64_t m_, n_;
    double *a_;

    a_ = mtralloc(lda, p);

    for (uint64_t i = 0; i < m; i++)
        for (uint64_t j = 0; j < p; j++)
            A_(i,j) = A(i,j);

    for (n_ = n; n_ % 4 != 0; n_--);
    for (m_ = m; m_ % 4 != 0; m_--); 

    for (uint64_t j = 0; j < n_; j += 4)
        for (uint64_t i = 0; i < m_; i += 4)
            dotprod_4x4(&A_(i,0), lda, &B(0,j), ldb, &C(i,j), ldc, p);

    for (uint64_t j = n_; j < n; j++)
        for (uint64_t i = 0; i < m; i++)
            dotprod(&A_(i,0), lda, &B(0,j), &C(i,j), p);

    for (uint64_t i = m_; i < m; i++)
        for (uint64_t j = 0; j < n_; j++)
            dotprod(&A_(i,0), lda, &B(0,j), &C(i,j), p);

    mtrfree(a_);
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

