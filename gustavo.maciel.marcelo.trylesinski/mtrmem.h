#ifndef MTRMEM_H
#define MTRMEM_H

#include <stdint.h>

/* Inicializa e devolve uma matriz double m por n. */
double **mtrinit(uint64_t m, uint64_t n);

/* Limpa o espaco ocupado pela matriz double mtr m por n. */
void mtrclear(double **mtr, uint64_t m, uint64_t n);

#endif
