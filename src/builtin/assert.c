#include "builtin.h"

scm_var_t scm_assert_true(scm_var_t args)
{
    int i;
    scm_var_t arg;
    bool ret = true;

    vec_foreach(&args._toks, arg, i)
    {
        if (arg.type != SCM_BOOL)
        {
            return scm_token_error(SCM_TYPE_ERROR,
                                   "assert-eq? only takes booleans");
        }

        ret &= arg._bool;
    }

    if (!ret)
    {
        return scm_token_error(SCM_ASSERT_ERROR, "\033[2D ");
    }

    return scm_token_nil;
}