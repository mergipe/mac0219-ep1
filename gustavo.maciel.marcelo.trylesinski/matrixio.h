#ifndef MATRIXIO_H
#define MATRIXIO_H

#include <stdint.h>
#include <stdio.h>

/* Devolve uma matriz double m por n, lida de mFile */
double **read_matrix(uint64_t *m, uint64_t *n, FILE *mFile);

/* Escreve a matriz double matrix, m por n, no arquivo mFile */
void print_matrix(double **matrix, uint64_t m, uint64_t n, FILE *mFile);

#endif
