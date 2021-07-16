#include "builtin.h"

scm_var_t scm_equal(scm_var_t args)
{
    enum scm_types type = args._toks.data[0].type;

    scm_var_t arg;
    int i;

    vec_foreach(&args._toks, arg, i)
    {
        if (arg.type != type)
        {
            return scm_token(false);
        }
    }

    return scm_token(true);
}