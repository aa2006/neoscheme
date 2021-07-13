#include <assert.h>
#include <stdio.h>
#include <assert.h>
#include <vec/vec.h>
#include <types.h>
#include <eval.h>

scm_var_t scm_load(scm_var_t args)
{
    if (args._toks.length == 0)
    {
        fprintf(stderr, "ArgumentError: missing arguments\n");
        return scm_token_nil;
    }

    if (args._toks.length > 1)
    {
        fprintf(stderr, "ArgumentError: too much arguments\n");
        return scm_token_nil;
    }

    if (args._toks.data[0].type != SCM_STR)
    {
        fprintf(stderr, "TypeError: Loads only takes a string\n");
        return scm_token_nil;
    }

    FILE *fp = fopen(args._toks.data[0]._str, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "FileNotFound: %s no such file\n", args._toks.data[0]._str);
        return scm_token_nil;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = (char *) malloc(size+1);
    assert(buffer != NULL);

    fread(buffer, (size_t) size, 1, fp);
    fclose(fp);

    buffer[size] = 0;
    scm_eval(buffer, false);

    free(buffer);
    return scm_token_nil;
}