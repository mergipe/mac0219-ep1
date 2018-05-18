#include "mtrio.h"
#include "mtrmem.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

double *readmtr(uint64_t *m, uint64_t *n, uint64_t *ld, FILE *mFile)
{
    uint64_t i_, j_;
    double *mtr, value;

    fscanf(mFile, "%" SCNu64, m);
    fscanf(mFile, "%" SCNu64, n);

    *ld = *m;

    mtr = mtralloc(*ld, *n);

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

void printmtr(uint64_t m, uint64_t n, double *mtr, uint64_t ld, FILE *mFile)
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
