#include "eval.h"

#include <stdio.h>
#include <stdlib.h>
#include <types.h>

static char *unescape(char const *str)
{
    char *ret = calloc(strlen(str), sizeof(char));

    for (size_t i = 0; i < strlen(str); i++)
    {
        if (str[i] == '\\')
        {
            switch (str[++i])
            {
                case 'n':
                {
                    ret[strlen(ret)] = '\n';
                    break;
                }
                case 't':
                {
                    ret[strlen(ret)] = '\t';
                    break;
                }
                default:
                {
                    ret[strlen(ret)] = str[i];
                }
            }
        }
        else
        {
            ret[strlen(ret)] = str[i];
        }
    }

    return ret;
}

scm_var_t scm_display(scm_var_t args)
{
    scm_var_t arg;
    int i;

    vec_foreach(&args._toks, arg, i)
    {
        switch (arg.type)
        {
            case SCM_NUMBER_INT:
            {
                printf("%ld", arg._nbr);
                break;
            }

            case SCM_NUMBER_FLOAT:
            {
                printf("%f", arg._float);
                break;
            }

            case SCM_STR:
            {
                char *s = unescape(arg._str);
                printf("%s", s);

                free(s);
                break;
            }

            case SCM_BOOL:
            {
                printf(arg._bool ? "#t" : "#f");
                break;
            }

            default:
            {
                printf("Non-printable");
                break;
            }
        }
    }

    return scm_token_nil;
}

scm_var_t scm_exit(scm_var_t args)
{
    if (args._toks.length == 0)
    {
        exit(0);
    }
    else if (args._toks.length == 1)
    {
        if (args._toks.data[0].type == SCM_NUMBER_INT ||
            args._toks.data[0].type == SCM_NUMBER_FLOAT)
        {
            exit(args._toks.data[0].type == SCM_NUMBER_FLOAT
                     ? (int) args._toks.data[0]._float
                     : (int) args._toks.data[0]._nbr);
        }
        else
        {
            return scm_token_error(SCM_TYPE_ERROR, "exit only takes a number");
        }
    }
    else
    {
        return scm_token_error(SCM_ARGUMENT_ERROR, "too much arguments");
    }

    return scm_token_nil;
}

scm_var_t scm_error(scm_var_t args)
{
    if (args._toks.length == 0)
    {
        return scm_token_error(SCM_SCHEME_ERROR, "?");
    }

    int i;
    scm_var_t arg;
    char *error = calloc(sizeof(char), 1);

    vec_foreach(&args._toks, arg, i)
    {
        error = realloc(error, strlen(error) + strlen(args._str) + 1);
        strcat(error, arg._str);
    }

    return scm_token_error(SCM_SCHEME_ERROR, error);
}