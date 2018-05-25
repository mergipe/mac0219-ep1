#ifndef MTRMEM_H
#define MTRMEM_H

#include <stdint.h>

#define ALIGN_SIZE 32

/* Aloca e devolve uma matriz double ld por od, nao inicializada, com leading
 * dimension ld. A matriz é alocada como um vetor, tal que
 * M(i,j) = mtr[j * ld + i]. */
double *mtralloc(uint64_t ld, uint64_t od);

/* Aloca e devolve uma matriz nula double ld por od, com leading dimension
 * ld. A matriz é alocada como um vetor, tal que M(i,j) = mtr[j * ld + i]. */
double *mtrcalloc(uint64_t ld, uint64_t od);

/* Limpa o espaco ocupado pela matriz double mtr. */
void mtrfree(double *mtr);

#endif
