#include <eval.h>
#include <stdio.h>
#include <types.h>

scm_var_t scm_define(scm_var_t args)
{
    if (args._toks.length < 2)
    {
        return scm_token_error(SCM_ARGUMENT_ERROR, "missing arguments");
    }
    else if (args._toks.length > 2)
    {
        return scm_token_error(SCM_ARGUMENT_ERROR, "too much arguments");
    }

    if (args._toks.data[0].type != SCM_SYMBOLS)
    {
        return scm_token_error(SCM_TYPE_ERROR,
                               "define takes a symbol as its first argument");
    }

    scm_c_define(args._toks.data[0]._str, args._toks.data[1]);
    return scm_token_nil;
}