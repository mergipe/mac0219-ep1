#ifndef MTRIO_H
#define MTRIO_H

#include <stdint.h>
#include <stdio.h>

/* Devolve uma matriz double m por n lida de mFile. */
double **readmtr(uint64_t *m, uint64_t *n, FILE *mFile);

/* Escreve a matriz double mtr m por n no arquivo mFile. */
void printmtr(double **mtr, uint64_t m, uint64_t n, FILE *mFile);

#endif
