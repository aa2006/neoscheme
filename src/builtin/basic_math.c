#include <stdio.h>
#include <types.h>
#include <stdlib.h>
#include <vec/vec.h>

scm_var_t scm_add(vec_scm_var_t args)
{
    scm_var_t ret;
    ret.type = SCM_NUMBER_INT;
    ret._nbr = 0;

    int i;
    scm_var_t number;

    vec_foreach(&args, number, i)
    {
        if (number.type != SCM_NUMBER_FLOAT && number.type != SCM_NUMBER_INT)
        {
            fprintf(stderr, "TypeError: Can only add Numbers\n");
            return scm_token(SCM_NIL, NULL);
        }

        if (number.type == SCM_NUMBER_FLOAT)
        {
            if (ret.type == SCM_NUMBER_INT)
            {
                size_t nbr = ret._nbr;
                ret._nbr = 0;
                ret._float += (double) nbr;
                ret.type = SCM_NUMBER_FLOAT;
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


scm_var_t scm_subs(vec_scm_var_t args)
{
    scm_var_t ret;
    ret.type = SCM_NUMBER_INT;

    if (args.data[0].type != SCM_NUMBER_FLOAT && args.data[0].type != SCM_NUMBER_INT)
    {
        fprintf(stderr, "TypeError: Can only add Numbers\n");
        return scm_token(SCM_NIL, NULL);
    }

    if (args.length == 1)
    {
        if (args.data[0].type == SCM_NUMBER_INT)
        {
            ret._nbr = 0 - args.data[0]._nbr;
        }
        else 
        {
            ret.type = SCM_NUMBER_FLOAT;
            ret._float = 0 - args.data[0]._float;
        }
    }
    else 
    {
        if (args.data[0].type == SCM_NUMBER_INT)
        {
            ret._nbr = args.data[0]._nbr;
        }
        else 
        {
            ret.type = SCM_NUMBER_FLOAT;
            ret._float = 0 - args.data[0]._float;
        }
    }


    for (int i = 1; i < args.length; i++)
    {
        if (args.data[i].type != SCM_NUMBER_FLOAT && args.data[i].type != SCM_NUMBER_INT)
        {
            fprintf(stderr, "TypeError: Can only add Numbers\n");
            return scm_token(SCM_NIL, NULL);
        }

        if (args.data[i].type == SCM_NUMBER_FLOAT)
        {
            if (ret.type == SCM_NUMBER_INT)
            {
                size_t nbr = ret._nbr;
                ret._nbr = 0;
                ret._float = (double) nbr;
                ret.type = SCM_NUMBER_FLOAT;
            }

            ret._float -= (double) args.data[i]._float;
        }
        else if(args.data[i].type == SCM_NUMBER_INT && ret.type == SCM_NUMBER_FLOAT)
        {
            ret._float -= (double) args.data[i]._nbr;
        }

        else 
        {
            ret._nbr -= args.data[i]._nbr;
        }

        ret._nbr -= args.data[i]._nbr;
    }

    return ret;
}