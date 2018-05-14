#ifndef MTRMUL_H
#define MTRMUL_H

#include <stdint.h>

#define OPENMP_IMPL 'o'
#define PTHREAD_IMPL 'p'

/* Devolve a matriz double (m por n) resultante da multiplicacao de mtr_A
 * (m por p) por mtr_B (p por n). Utiliza o método iterativo baseado na
 * definicao de multiplicacao entre matrizes. */
double **mtrmul_naive(double **mtr_A, double **mtr_B, uint64_t m, uint64_t p,
                      uint64_t n);

#endif
