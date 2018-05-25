#include "mtrio.h"
#include "mtrmem.h"
#include "mtrmul.h"

#include <argp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static char doc[] =
    "This program computes the matrix multiplication C = A * B.\v"
    "You have to choose exactly one of the OPTIONS.";

static char args_doc[] = "FILE_A FILE_B FILE_C";

static struct argp_option options[] = 
{
    {0,        0,  0, 0,                            "OPTIONS:"},
    {0,       'o', 0, 0,                            "Run the OpenMP implementation"},
    {0,       'p', 0, 0,                            "Run the pthread implementation"},
    {0,        0,  0, 0,                            "ARGS:"},
    {"FILE_A", 0,  0, OPTION_DOC | OPTION_NO_USAGE, "Path to matrix A file"},
    {"FILE_B", 0,  0, OPTION_DOC | OPTION_NO_USAGE, "Path to matrix B file"},
    {"FILE_C", 0,  0, OPTION_DOC | OPTION_NO_USAGE, "Path to matrix C file"},
    {0}
};

struct arguments
{
    char *args[3];
    int openmp, pthread;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch(key)
    {
        case 'o':
            arguments->openmp = 1;
            break;
        case 'p':
            arguments->pthread = 1;
            break;

        case ARGP_KEY_ARG:
            if (state->arg_num >= 3)
                argp_state_help(state, stderr, ARGP_HELP_STD_HELP);
            arguments->args[state->arg_num] = arg;
            break;

        case ARGP_KEY_END:
            if (state->arg_num < 3)
                argp_state_help(state, stderr, ARGP_HELP_STD_HELP);
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    if (arguments->openmp == arguments->pthread)
        argp_state_help(state, stderr, ARGP_HELP_STD_HELP);

    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char **argv)
{
    double *a, *b, *c;
    uint64_t m, p, n, lda, ldb, ldc;
    FILE *aFile, *bFile, *cFile;
    struct arguments arguments;

    arguments.openmp = 0;
    arguments.pthread = 0;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    aFile = fopen(arguments.args[0], "r");
    if (aFile == NULL)
    {
        perror(arguments.args[0]);
        return EXIT_FAILURE;
    }

    bFile = fopen(arguments.args[1], "r");
    if (bFile == NULL)
    {
        perror(arguments.args[1]);
        return EXIT_FAILURE;
    }

    cFile = fopen(arguments.args[2], "w");
    if (cFile == NULL)
    {
        perror(arguments.args[2]);
        return EXIT_FAILURE;
    }

    a = readmtr(&m, &p, &lda, aFile);
    fclose(aFile);
    b = readmtr(&p, &n, &ldb, bFile);
    fclose(bFile);

    ldc = m;
    c = mtrcalloc(ldc, n);

    if (arguments.openmp)
        mtrmul_opt_o(m, p, n, a, lda, b, ldb, c, ldc);
    else
        mtrmul_opt_p(m, p, n, a, lda, b, ldb, c, ldc);

    printmtr(m, n, c, ldc, cFile);
    fclose(cFile);

    mtrfree(a);
    mtrfree(b);
    mtrfree(c);

    return EXIT_SUCCESS;
}
