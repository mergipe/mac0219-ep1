#include "mtrio.h"
#include "mtrmem.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#define M_(i,j) mtr[j * (*m) + i]
#define M(i,j) mtr[j * m + i]

double *readmtr(uint64_t *m, uint64_t *n, FILE *mFile)
{
    uint64_t i_, j_;
    double *mtr, value;

    fscanf(mFile, "%" SCNu64, m);
    fscanf(mFile, "%" SCNu64, n);

    mtr = mtralloc(*m, *n);

    fscanf(mFile, "%" SCNu64, &i_);
    fscanf(mFile, "%" SCNu64, &j_);
    fscanf(mFile, "%lf", &value);

    for (uint64_t i = 0; i < *m; i++)
    {
        for (uint64_t j = 0; j < *n; j++)
        {
            if (i + 1 == i_ && j + 1 == j_)
            {
                M_(i,j) = value;

                if (!feof(mFile))
                {
                    fscanf(mFile, "%" SCNu64, &i_);
                    fscanf(mFile, "%" SCNu64, &j_);
                    fscanf(mFile, "%lf", &value);
                }
            }
            else
            {
                M_(i,j) = 0;
            }
        }
    }

    return mtr;
}

void printmtr(double *mtr, uint64_t m, uint64_t n, FILE *mFile)
{
    fprintf(mFile, "%" PRIu64 " %" PRIu64, m, n);

    for (uint64_t i = 0; i < m; i++)
    {
        for (uint64_t j = 0; j < n; j++)
        {
            if (M(i,j) != 0)
            {
                fprintf(mFile, "\n%" PRIu64 " %" PRIu64 " %f",
                        i + 1, j + 1, M(i,j));
            }
        }
    }

    fprintf(mFile, "\n");
}
