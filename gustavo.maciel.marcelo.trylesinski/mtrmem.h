#ifndef MTRMEM_H
#define MTRMEM_H

#include <stdint.h>

/* Aloca e devolve uma matriz double m por n, nao inicializada. */
double *mtralloc(uint64_t m, uint64_t n);

/* Aloca e devolve uma matriz nula double m por n. */
double *mtrcalloc(uint64_t m, uint64_t n);

/* Limpa o espaco ocupado pela matriz double mtr. */
void mtrfree(double *mtr);

#endif
