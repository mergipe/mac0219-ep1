#ifndef MTRMUL_H
#define MTRMUL_H

#include <stdint.h>

#define OPENMP_IMPL 'o'
#define PTHREAD_IMPL 'p'

/* Computa c = a * b + c, sendo a, b e c matrizes double (m por p), (p por n)
 * e (m por n), respectivamente. */
void mtrmul_naive(double *a, double *b, double *c,
                  uint64_t m, uint64_t p, uint64_t n);

/* Computa c = a * b + c, sendo a, b e c matrizes double (m por p), (p por n)
 * e (m por n), respectivamente. */
void mtrmul_opt(double *a, double *b, double *c,
                uint64_t m, uint64_t p, uint64_t n);

#endif
