#include <stdio.h>
#include <types.h>
#include "eval.h"

scm_var_t scm_display(scm_var_t args)
{
    scm_var_t ret = scm_token_nil;

    if (args._toks.length > 1)
    {
        fprintf(stderr, "ArgumentError: too much arguments\n");
        return ret;
    }

    scm_var_t first_arg = args._toks.data[0];

    switch (first_arg.type)
    {
        case SCM_NUMBER_INT:
        {
            printf("%ld\n", first_arg._nbr);
            break;
        }

        case SCM_NUMBER_FLOAT:
        {
            printf("%f\n", first_arg._float);
            break;
        }

        case SCM_STR:
        {
            printf("%s\n", first_arg._str);
            break;
        }

        case SCM_BOOL:
        {
            printf(first_arg._bool ? "#t" : "#f");
            break;
        }

        default:
        {
            printf("Non-printable");
            break;
        }
    }

    return ret;
}