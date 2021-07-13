#include <stdio.h>
#include <types.h>
#include <stdlib.h>
#include <vec/vec.h>

scm_var_t scm_add(scm_var_t args)
{
    scm_var_t ret = scm_token(0);

    int i;
    scm_var_t number;

    vec_foreach(&args._toks, number, i)
    {
        if (number.type != SCM_NUMBER_FLOAT && number.type != SCM_NUMBER_INT)
        {
            fprintf(stderr, "TypeError: Can only add Numbers\n");
            return scm_token_nil;
        }

        if (number.type == SCM_NUMBER_FLOAT)
        {
            if (ret.type == SCM_NUMBER_INT)
            {
                ret = scm_token(ret._nbr * 1.0);
            }

            ret._float += (double) number._float;
        }
        else if(number.type == SCM_NUMBER_INT && ret.type == SCM_NUMBER_FLOAT)
        {
            ret._float += (double) number._nbr;
        }

        else 
        {
            ret._nbr += number._nbr;
        }
    }

    return ret;
}

scm_var_t scm_subs(scm_var_t args)
{
    scm_var_t ret;
    ret.type = SCM_NUMBER_INT;

    if (args._toks.data[0].type != SCM_NUMBER_FLOAT && args._toks.data[0].type != SCM_NUMBER_INT)
    {
        fprintf(stderr, "TypeError: Can only add Numbers\n");
        return scm_token_nil;
    }

    if (args._toks.length == 1)
    {
        if (args._toks.data[0].type == SCM_NUMBER_INT)
        {
            ret._nbr = 0 - args._toks.data[0]._nbr;
        }
        else 
        {
            ret.type = SCM_NUMBER_FLOAT;
            ret._float = 0 - args._toks.data[0]._float;
        }

        return ret;
    }
    else 
    {
        if (args._toks.data[0].type == SCM_NUMBER_INT)
        {
            ret._nbr = args._toks.data[0]._nbr;
        }
        else 
        {
            ret.type = SCM_NUMBER_FLOAT;
            ret._float = 0 - args._toks.data[0]._float;
        }
    }


    for (int i = 1; i < args._toks.length; i++)
    {
        if (args._toks.data[i].type != SCM_NUMBER_FLOAT && args._toks.data[i].type != SCM_NUMBER_INT)
        {
            fprintf(stderr, "TypeError: Can only add Numbers\n");
            return scm_token_nil;
        }

        if (args._toks.data[i].type == SCM_NUMBER_FLOAT)
        {
            if (ret.type == SCM_NUMBER_INT)
            {
                size_t nbr = ret._nbr;
                ret._nbr = 0;
                ret._float = (double) nbr;
                ret.type = SCM_NUMBER_FLOAT;
            }

            ret._float -= (double) args._toks.data[i]._float;
        }
        else if(args._toks.data[i].type == SCM_NUMBER_INT && ret.type == SCM_NUMBER_FLOAT)
        {
            ret._float -= (double) args._toks.data[i]._nbr;
        }
        else 
        {
            ret._nbr -= args._toks.data[i]._nbr;
        }
    }

    return ret;
}