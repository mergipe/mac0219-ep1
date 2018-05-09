#include "matrixio.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

double **read_matrix(uint64_t *m, uint64_t *n, FILE *mFile)
{
    uint64_t i_, j_;
    double **matrix, value;

    fscanf(mFile, "%" SCNu64, m);
    fscanf(mFile, "%" SCNu64, n);

    matrix = malloc(*m * sizeof (double *));
    for (uint64_t i = 0; i < *m; i++)
        matrix[i] = malloc(*n * sizeof (double));

    fscanf(mFile, "%" SCNu64, &i_);
    fscanf(mFile, "%" SCNu64, &j_);
    fscanf(mFile, "%lf", &value);

    for (uint64_t i = 0; i < *m; i++)
    {
        for (uint64_t j = 0; j < *n; j++)
        {
            if (i + 1 == i_ && j + 1 == j_)
            {
                matrix[i][j] = value;

                if (!feof(mFile))
                {
                    fscanf(mFile, "%" SCNu64, &i_);
                    fscanf(mFile, "%" SCNu64, &j_);
                    fscanf(mFile, "%lf", &value);
                }
            }
            else
            {
                matrix[i][j] = 0;
            }
        }
    }

    return matrix;
}

void print_matrix(double **matrix, uint64_t m, uint64_t n, FILE *mFile)
{
    fprintf(mFile, "%" PRIu64 " %" PRIu64, m, n);

    for (uint64_t i = 0; i < m; i++)
    {
        for (uint64_t j = 0; j < n; j++)
        {
            if (matrix[i][j] != 0)
            {
                fprintf(mFile,
                        "\n%" PRIu64 " %" PRIu64 " %f",
                        i + 1, j + 1, matrix[i][j]);
            }
        }
    }

    fprintf(mFile, "\n");
}
