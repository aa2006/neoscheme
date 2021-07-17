#include "builtin.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

scm_var_t scm_equal(scm_var_t args)
{
    bool return_value;

    scm_var_t first_val = vec_pop(&args._toks);
    scm_var_t second_val = vec_pop(&args._toks);

    if (first_val.type == SCM_NUMBER_INT)
    {
        first_val = scm_token((double) first_val._nbr);
    }

    if (second_val.type == SCM_NUMBER_INT)
    {
        second_val = scm_token((double) second_val._nbr);
    }

    if (first_val.type != second_val.type)
    {
        return scm_token(false);
    }

    switch (first_val.type)
    {
        case SCM_NUMBER_FLOAT:
        {
            return_value = first_val._float == second_val._float;
            break;
        }
        case SCM_BOOL:
        {
            return_value = first_val._bool == second_val._bool;
            break;
        }
        case SCM_STR:
        {
            return_value = strcmp(first_val._str, second_val._str) == 0;
            break;
        }
        case SCM_NIL:
        {
            return_value = true;
            break;
        }
        default:
        {
            char error[23];

            sprintf(error, "can't compare %s", scm_typename[first_val.type]);

            return scm_token_error(SCM_TYPE_ERROR, error);
        }
    }

    if (args._toks.length > 0)
    {
        if (args._toks.length == 1)
        {
            assert(vec_push(&args._toks, first_val) == 0);
        }
        return_value &= scm_equal(args)._bool;
    }

    return scm_token(return_value);
}