#ifndef MTRMULH
#define MTRMUL_H

#include <stdint.h>

/* Computa c = a * b + c, sendo a, b e c matrizes double (m por p), (p por n)
 * e (m por n), respectivamente. As leading dimensions de a, b e c sao lda,
 * ldb e ldc, respectivamente. Utiliza o algoritmo 'naive'. */
void mtrmul_naive(uint64_t m, uint64_t p, uint64_t n,
                  double *a, uint64_t lda,
                  double *b, uint64_t ldb,
                  double *c, uint64_t ldc);

/* Computa c = a * b + c, sendo a, b e c matrizes double (m por p), (p por n)
 * e (m por n), respectivamente. As leading dimensions de a, b e c sao lda,
 * ldb e ldc, respectivamente. Utiliza um algoritmo otimizado baseado no
 * 'naive' e OpenMP. */
void mtrmul_opt_o(uint64_t m, uint64_t p, uint64_t n,
                  double *a, uint64_t lda,
                  double *b, uint64_t ldb,
                  double *c, uint64_t ldc);

/* Computa c = a * b + c, sendo a, b e c matrizes double (m por p), (p por n)
 * e (m por n), respectivamente. As leading dimensions de a, b e c sao lda,
 * ldb e ldc, respectivamente. Utiliza um algoritmo otimizado baseado no
 * 'naive' e pthreads. */
void mtrmul_opt_p(uint64_t m, uint64_t p, uint64_t n,
                  double *a, uint64_t lda,
                  double *b, uint64_t ldb,
                  double *c, uint64_t ldc);

#endif
