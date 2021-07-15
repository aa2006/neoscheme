#include <eval.h>
#include <stdio.h>
#include <types.h>

scm_var_t scm_define(scm_var_t args)
{
    if (args._toks.length < 2)
    {
        fprintf(stderr, "ArgumentError: missing arguments\n");
        return scm_token_nil;
    }
    else if (args._toks.length > 2)
    {
        fprintf(stderr, "ArgumentError: too much arguments\n");
        return scm_token_nil;
    }

    if (args._toks.data[0].type != SCM_SYMBOLS)
    {
        fprintf(stderr, "TypeError: need one symbol\n");
        return scm_token_nil;
    }

    scm_c_define(args._toks.data[0]._str, args._toks.data[1]);
    return scm_token_nil;
}