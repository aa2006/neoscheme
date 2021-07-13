#include <types.h>
#include <stdlib.h>
#include <vec/vec.h>

scm_var_t scm_add(vec_str_t args)
{
    scm_var_t ret;
    ret.type = SCM_NUMBER_INT;
    ret._nbr = 0;

    int i;
    char *number;

    vec_foreach(&args, number, i)
    {
        size_t nbr = atoll(number);
        ret._nbr += nbr;
    }

    return ret;
}


scm_var_t scm_subs(vec_str_t args)
{
    scm_var_t ret;
    ret.type = SCM_NUMBER_INT;
    ret._nbr = atoll(args.data[0]);

    for (int i = 1; i < args.length; i++)
    {
        size_t nbr = atoll(args.data[i]);
        ret._nbr -= nbr;
    }

    return ret;
}