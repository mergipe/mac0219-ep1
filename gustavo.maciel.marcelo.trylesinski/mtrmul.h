#ifndef MTRMULH
#define MTRMUL_H

#include <stdint.h>

#define OPENMP_IMPL 'o'
#define PTHREAD_IMPL 'p'

#define A(i,j)  a[j * lda + i]
#define B(i,j)  b[j * ldb + i]
#define C(i,j)  c[j * ldc + i]
#define A_(i,j) a_[i * lda + j]
#define a(k)    a[k]
#define b(k)    b[k]

/* Computa c = a * b + c, sendo a, b e c matrizes double (m por p), (p por n)
 * e (m por n), respectivamente. As leading dimensions de a, b e c sao lda,
 * ldb e ldc, respectivamente. */
void mtrmul_naive(uint64_t m, uint64_t p, uint64_t n,
                  double *a, uint64_t lda,
                  double *b, uint64_t ldb,
                  double *c, uint64_t ldc);

/* Computa c = a * b + c, sendo a, b e c matrizes double (m por p), (p por n)
 * e (m por n), respectivamente. As leading dimensions de a, b e c sao lda,
 * ldb e ldc, respectivamente. Utiliza OpenMP. */
void mtrmul_opt_o(uint64_t m, uint64_t p, uint64_t n,
                  double *a, uint64_t lda,
                  double *b, uint64_t ldb,
                  double *c, uint64_t ldc);

/* Computa c = a * b + c, sendo a, b e c matrizes double (m por p), (p por n)
 * e (m por n), respectivamente. As leading dimensions de a, b e c sao lda,
 * ldb e ldc, respectivamente. Utiliza pthreads. */
void mtrmul_opt_p(uint64_t m, uint64_t p, uint64_t n,
                  double *a, uint64_t lda,
                  double *b, uint64_t ldb,
                  double *c, uint64_t ldc);


#endif
