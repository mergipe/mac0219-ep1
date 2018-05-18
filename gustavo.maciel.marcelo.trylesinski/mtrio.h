#ifndef MTRIO_H
#define MTRIO_H

#include <stdint.h>
#include <stdio.h>

#define M_(i,j) mtr[j * (*ld) + i]
#define M(i,j) mtr[j * ld + i]

/* Devolve uma matriz double m por n, com leading dimension ld, lida de
 * mFile. */
double *readmtr(uint64_t *m, uint64_t *n, uint64_t *ld, FILE *mFile);

/* Escreve a matriz double mtr m por n, com leading dimension ld, no arquivo
 * mFile. */
void printmtr(uint64_t m, uint64_t n, double *mtr, uint64_t ld, FILE *mFile);

#endif
