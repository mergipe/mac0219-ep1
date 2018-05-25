#include "mtrmul.h"
#include "mtrmem.h"

#include <omp.h>
#include <stdint.h>
#include <immintrin.h>

/* Macros para acessar uma posicao (i,j) das matrizes. */
#define  A(i,j)  a[(j) * lda  + (i)]
#define  B(i,j)  b[(j) * ldb  + (i)]
#define  C(i,j)  c[(j) * ldc  + (i)]
#define A_(i,j) a_[(i) * lda_ + (j)]

/* Macros para acessar uma posicao i dos vetores (somente para deixar a notacao
 * padronizada). */
#define a(k) a[k]
#define b(k) b[k]

/* Computa o produto interno c = a * b + c entre os vetores a e b de tamanho
 * p. */
static void dotprod(double *a, double *b, double *c, uint64_t p)
{
    for (uint64_t k = 0; k < p; k++)
        *c += a(k) * b(k);
}

/* Computa 4x4 produtos internos C(i,j) = A(i,k) * B(k,j) + C(i,j), com
 * 0 <= i,j <= 3 e 0 <= k <= p. */
static void dotprod_4x4(double *a_, uint64_t lda_, double *b, uint64_t ldb,
                        double *c, uint64_t ldc, uint64_t p)
{
    __m256d c_i0_vreg, c_i1_vreg, c_i2_vreg, c_i3_vreg,
            a_ik_vreg,
            b_k0_vreg, b_k1_vreg, b_k2_vreg, b_k3_vreg;

    double *b_k0_pntr, *b_k1_pntr, *b_k2_pntr, *b_k3_pntr;

    b_k0_pntr = &B(0,0);
    b_k1_pntr = &B(0,1);
    b_k2_pntr = &B(0,2);
    b_k3_pntr = &B(0,3);

    c_i0_vreg = _mm256_load_pd(&C(0,0));
    c_i1_vreg = _mm256_load_pd(&C(0,1));
    c_i2_vreg = _mm256_load_pd(&C(0,2));
    c_i3_vreg = _mm256_load_pd(&C(0,3));

    for (uint64_t k = 0; k < p; k++)
    {
        a_ik_vreg = _mm256_set_pd(A_(3,k), A_(2,k), A_(1,k), A_(0,k));

        b_k0_vreg = _mm256_set1_pd(*b_k0_pntr++);
        b_k1_vreg = _mm256_set1_pd(*b_k1_pntr++);
        b_k2_vreg = _mm256_set1_pd(*b_k2_pntr++);
        b_k3_vreg = _mm256_set1_pd(*b_k3_pntr++);

        c_i0_vreg = _mm256_add_pd(c_i0_vreg, 
                                  _mm256_mul_pd(a_ik_vreg, b_k0_vreg));
        c_i1_vreg = _mm256_add_pd(c_i1_vreg, 
                                  _mm256_mul_pd(a_ik_vreg, b_k1_vreg));
        c_i2_vreg = _mm256_add_pd(c_i2_vreg, 
                                  _mm256_mul_pd(a_ik_vreg, b_k2_vreg));
        c_i3_vreg = _mm256_add_pd(c_i3_vreg, 
                                  _mm256_mul_pd(a_ik_vreg, b_k3_vreg));
    }

    _mm256_store_pd(&C(0,0), c_i0_vreg);
    _mm256_store_pd(&C(0,1), c_i1_vreg);
    _mm256_store_pd(&C(0,2), c_i2_vreg);
    _mm256_store_pd(&C(0,3), c_i3_vreg);
}

void mtrmul_opt_o(uint64_t m, uint64_t p, uint64_t n,
                  double *a, uint64_t lda,
                  double *b, uint64_t ldb,
                  double *c, uint64_t ldc)
{
    uint64_t m_, n_, lda_;
    double *a_;

    /* A matriz a_ é alocada em row major para melhorar o acesso ao cache */
    lda_ = ldb;
    a_ = mtralloc(m, lda_);

    for (uint64_t i = 0; i < m; i++)
        for (uint64_t j = 0; j < p; j++)
            A_(i,j) = A(i,j);

    /* Calcula os numeros n_ <= n e m_ <= m mais proximos de n e m,
     * respectivamente, e que sao divisiveis por 4 */
    for (n_ = n; n_ % 4 != 0; n_--);
    for (m_ = m; m_ % 4 != 0; m_--); 

    /* Computa blocos 4x4 da matriz c, em paralelo */
    #pragma omp parallel for
    for (uint64_t j = 0; j < n_; j += 4)
        for (uint64_t i = 0; i < m_; i += 4)
            dotprod_4x4(&A_(i,0), lda_, &B(0,j), ldb, &C(i,j), ldc, p);

    /* Computa as colunas restantes */
    for (uint64_t j = n_; j < n; j++)
        for (uint64_t i = 0; i < m; i++)
            dotprod(&A_(i,0), &B(0,j), &C(i,j), p);

    /* Computa as linhas restantes */
    for (uint64_t i = m_; i < m; i++)
        for (uint64_t j = 0; j < n_; j++)
            dotprod(&A_(i,0), &B(0,j), &C(i,j), p);

    mtrfree(a_);
}

static void dotprod_4x4_p(double *a_, uint64_t lda_, double *b, uint64_t ldb,
                          double *c, uint64_t ldc, uint64_t p)
{
    /* Um registrador para cada posicao de c a ser computada e um registrador
     * para cada linha de a */
    register double c_00_reg, c_01_reg, c_02_reg, c_03_reg,
                    c_10_reg, c_11_reg, c_12_reg, c_13_reg,
                    c_20_reg, c_21_reg, c_22_reg, c_23_reg,
                    c_30_reg, c_31_reg, c_32_reg, c_33_reg,

                    a_0k_reg,
                    a_1k_reg,
                    a_2k_reg,
                    a_3k_reg;

    /* Um ponteiro para cada coluna de b */
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

        /* Computa a primeira linha de c */
        c_00_reg += a_0k_reg * *b_k0_pntr;
        c_01_reg += a_0k_reg * *b_k1_pntr;
        c_02_reg += a_0k_reg * *b_k2_pntr;
        c_03_reg += a_0k_reg * *b_k3_pntr;

        /* Computa a segunda linha de c */
        c_10_reg += a_1k_reg * *b_k0_pntr;
        c_11_reg += a_1k_reg * *b_k1_pntr;
        c_12_reg += a_1k_reg * *b_k2_pntr;
        c_13_reg += a_1k_reg * *b_k3_pntr;

        /* Computa a terceira linha de c */
        c_20_reg += a_2k_reg * *b_k0_pntr;
        c_21_reg += a_2k_reg * *b_k1_pntr;
        c_22_reg += a_2k_reg * *b_k2_pntr;
        c_23_reg += a_2k_reg * *b_k3_pntr;

        /* Computa a quarta linha de c */
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

void mtrmul_opt_p(uint64_t m, uint64_t p, uint64_t n,
                  double *a, uint64_t lda,
                  double *b, uint64_t ldb,
                  double *c, uint64_t ldc)
{
    uint64_t m_, n_, lda_;
    double *a_;

    lda_ = ldb;
    a_ = mtralloc(m, lda_);

    for (uint64_t i = 0; i < m; i++)
        for (uint64_t j = 0; j < p; j++)
            A_(i,j) = A(i,j);

    for (n_ = n; n_ % 4 != 0; n_--);
    for (m_ = m; m_ % 4 != 0; m_--); 

    for (uint64_t j = 0; j < n_; j += 4)
        for (uint64_t i = 0; i < m_; i += 4)
            dotprod_4x4_p(&A_(i,0), lda_, &B(0,j), ldb, &C(i,j), ldc, p);

    for (uint64_t j = n_; j < n; j++)
        for (uint64_t i = 0; i < m; i++)
            dotprod(&A_(i,0), &B(0,j), &C(i,j), p);

    for (uint64_t i = m_; i < m; i++)
        for (uint64_t j = 0; j < n_; j++)
            dotprod(&A_(i,0), &B(0,j), &C(i,j), p);

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

